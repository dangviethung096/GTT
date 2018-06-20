#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "HashTable.h"
#include "ReadData.h"
#include "TernaryTree.h"
#include "GeneratorData.h"
#include "Wildcard.h"

#define MAX_IN_ITERATOR 5000000
//global variable
GTT *gtt_table;
/* Function just for test gtt_table */
void show_gtt_table(GTT *gtt_table, int length)
{
    int i;
    for(i = 0; i < length; i++)
    {
        printf("gtt_digit[%d] = %s\n", i, gtt_table[i].gtt_digit);
    }
}

GTT *table_digit;
//array for set of test
U8bit **test_set;
//variable for hash table
hash_table table[TABLE_LENGTH];
//variable for root node
struct Node *root = NULL;

//prototype
void search_single();
void search_wildcard();


//main
int main(){
    //search a single_gtt set
    // search_single();
    /* search wildcard */
    search_wildcard();
    // test_wildcard();
    return 1;
}

void search_single()
{
    //declare var to evaluate time
    clock_t start, end;
    double t;
    srand(time(NULL));
    //variable for loop
    int j;
    unsigned long i;
    //Read data from file
    read_data(&gtt_table, SINGLE_FILE);
    
    //Get test_set
    read_test(&test_set, TEST_SINGLE_FILE);
    //Make hastable to calculate
    for(i = 0; i < TABLE_LENGTH; i++){
        table[i] = NULL;
    }
    for(i = 0; i < NUMBER_GTT; i++)
    {
        add_bucket(table, gtt_table[i].gtt_digit, i);
    }
    write_table(table, SINGLE_FILE);
    
    //Calculate time with hash table
    start = clock();
    for(i = 0; i < NUMBER_TEST; i++)
    {
        search_key(table, test_set[i]);
    }
    end = clock();
    t = (double) (end - start) / CLOCKS_PER_SEC;
    printf("Thoi gian chay cua hash_table la : %f\n", t);
    delete_table(table);


    //Calculate time with iterator
    //Only run when test < MAX_IN_ITERATOR iterator because time of run is too long
    if(NUMBER_TEST <= MAX_IN_ITERATOR){
        start = clock();
        for(i = 0; i < NUMBER_TEST; i++){
            for(j = 0; j < NUMBER_GTT; j++){
                if(compare_gtt_digit(test_set[i], gtt_table[j].gtt_digit) == EQUAL){
                    break;
                }
            }
        }
        end = clock();
        t = (double) (end - start) / CLOCKS_PER_SEC;
        printf("Thoi gian chay cua vong for la : %f\n", t);
    }
    
    
    //Calculate with ternary tree
    for(i=0; i < NUMBER_GTT; i++)
	{
		insert_TST(&root,gtt_table[i].gtt_digit,i);
	}

    int index_in_gtt_table;
    start = clock();
    for(i = 0; i < NUMBER_TEST; i++){
        search_TST_dynamic(root, test_set[i], &index_in_gtt_table);
    }
    end = clock();
    t = (double) (end - start) / CLOCKS_PER_SEC;
    printf("Thoi gian chay cua ternary tree la : %f\n", t);
    delete_all_TST_node(&root);
}
/* Search with wildcard gtt and wildcard test */
void search_wildcard()
{
    //declare var to evaluate time
    clock_t start, end;
    double t;
    srand(time(NULL));
    //variable for loop
    int i;
    //Read data from file
    read_data(&gtt_table, WILDCARD_FILE);
    
    // show_gtt_table(gtt_table);
    //Get test_set
    read_test(&test_set, TEST_WILDCARD_FILE);
    
    int fail, succ;

    /* calculate time with linear search */
    fail = succ = 0;
    start = clock();
    for(i = 0; i < NUMBER_WILDCARD_TEST; i++)
    {
        int ret = search_linear_wildcard(gtt_table, test_set[i]);
        if(ret >= 0)
        {
            succ++;
            // printf("test_set[%d] = %s, gtt_table[%d] = %s\n", i, test_set[i], ret, gtt_table[ret].gtt_digit);
        }else
        {
            fail++;
            printf("test_set[%d] = %s,  FAIL!\n", i, test_set[i]);
        }
    }
    end = clock();
    t = (double) (end - start) / CLOCKS_PER_SEC;
    printf("Thoi gian chay cua tim kiem tuyen tinh la : %f\n", t);
    printf("fail = %d, success = %d\n", fail, succ);
    
    
    
    //Calculate with ternary tree
    for(i=0; i < NUMBER_WILDCARD; i++)
	{
		insert_TST(&root,gtt_table[i].gtt_digit,i);
    }
    /* Draw a tree using graphviz to draw */
    draw_tree(root, NULL);

    /* Count number found and not found */
    fail = succ = 0;
    int index_in_gtt_table;
    start = clock();
    for(i = 0; i < NUMBER_WILDCARD_TEST; i++){
        if(i == 2177)
        {
            printf("\n");
        }
        /*  */
        int ret = search_TST_wildcard(root, test_set[i], &index_in_gtt_table);
        if(ret == 0) {
            fail++;
            int result = search_linear_wildcard(gtt_table, test_set[i]);
            printf("test_set[%d] = %s, FAIL! gtt_result = %s\n", i, test_set[i], gtt_table[result].gtt_digit);
            
        }
        else
        {
            succ++;
            // printf("test_set[%d] = %s, gtt_table[%d] = %s\n", i, test_set[i], index_in_gtt_table, gtt_table[index_in_gtt_table].gtt_digit);
        }
            
    }
    end = clock();
    t = (double) (end - start) / CLOCKS_PER_SEC;
    printf("Thoi gian chay cua ternary tree la : %f\n", t);
    printf("fail = %d, success %d\n", fail, succ);
    delete_all_TST_node(&root);
}