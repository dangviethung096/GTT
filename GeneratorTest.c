#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "GTT.h"
#include "GeneratorTest.h"


int main()
{
    gen_single_test();
    gen_wildcard_test();
    return 1;
}

void gen_single_test()
{
    int i, j, k;
    time_t t;
    srand(time(&t));
    //read data
    GTT *gtt_table = (GTT *) calloc(NUMBER_GTT, sizeof(GTT));
    FILE *data = fopen(SINGLE_FILE, "r");
    fread(gtt_table, sizeof(GTT), NUMBER_GTT, data);
    fclose(data);
    //Generator test
    int fail_number = ceil(FAIL_PERCENT * NUMBER_TEST);
    int count = 0;
    U8bit gtt_digit[NUMBER_DIGIT];

    FILE *test = fopen(TEST_SINGLE_FILE, "w");
    //Loop to fill all test_fail in set
    for(i = 0; i < fail_number; i++)
    {
        int index = count + rand() % (NUMBER_TEST - count - fail_number + i);
        for(j = count; j < index; j++){
            int gtt_test = rand()%NUMBER_GTT;
            strcpy((char *) gtt_digit, (char *) gtt_table[gtt_test].gtt_digit);
            fwrite(gtt_digit, sizeof(U8bit), NUMBER_DIGIT, test);
        }
        for(k = 0; k < NUMBER_DIGIT - 1;k++){
            gtt_digit[k] = 48 + rand()%10;
        }
        gtt_digit[NUMBER_DIGIT - 1] = '\0';
        fwrite(gtt_digit, sizeof(U8bit), NUMBER_DIGIT, test);
        count = index + 1;
    }
    //fill rest of test
    for(;count < NUMBER_TEST; count++)
    {
        int gtt_test = rand()%NUMBER_GTT;
        strcpy((char *) gtt_digit, (char *) gtt_table[gtt_test].gtt_digit);
        fwrite(gtt_digit, sizeof(U8bit), NUMBER_DIGIT, test);
    }

    fclose(test);
    free(gtt_table);
}

int random_digit()
{
    return '0' + rand()%10;
}

void get_gtt_from_wildcard(U8bit *gtt_gen, U8bit *gtt_return)
{
    int length = strlen((const char *) gtt_gen);
    //gen digit
    int i, count  = 0;
    for(i = 0; i < length; i++)
    {
        if(gtt_gen[i] == '*')
        {
            int rest = NUMBER_DIGIT - count - length + i;
            /* Generator rest of * */
            while(rest > 0)
            {
                if(rand()%2)
                {
                    gtt_return[count++] = random_digit();
                }
                rest--;
            }
        }else if(gtt_gen[i] == '?')
        {
            gtt_return[count++] = random_digit();
        }else
        {//it is a number
            gtt_return[count++] = gtt_gen[i];
        }
    }
    gtt_return[count] = '\0';
}

void gen_wildcard_test()
{
    int i, j, k;
    time_t t;
    srand(time(&t));
    //read data
    GTT *gtt_table = (GTT *) calloc(NUMBER_WILDCARD, sizeof(GTT));
    FILE *data = fopen(WILDCARD_FILE, "r");
    fread(gtt_table, sizeof(GTT), NUMBER_WILDCARD, data);
    fclose(data);
    //Generator test
    int fail_number =ceil(FAIL_PERCENT * NUMBER_WILDCARD_TEST);
    int count = 0;
    U8bit gtt_digit[NUMBER_DIGIT];

    FILE *test = fopen(TEST_WILDCARD_FILE, "w");
    //Loop to fill all test_fail in set
    for(i = 0; i < fail_number; i++)
    {
        int index = count + rand() % (NUMBER_WILDCARD_TEST - count - fail_number + i);
        for(j = count; j < index; j++){
            /* Generator a sucess digit */
            random_gtt_digit_from_wildcard(gtt_table, gtt_digit);
            /* Write digit to file */
            fwrite(gtt_digit, sizeof(U8bit), NUMBER_DIGIT, test);
        }
        /* Write a fail digit to file */
        for(k = 0; k < NUMBER_DIGIT - 1;k++){
            gtt_digit[k] = 48 + rand()%10;
        }
        gtt_digit[NUMBER_DIGIT - 1] = '\0';
        fwrite(gtt_digit, sizeof(U8bit), NUMBER_DIGIT, test);
        count = index + 1;
    }
    /* fill rest of test */
    for(;count < NUMBER_WILDCARD_TEST; count++)
    {
        /* Generator a success digit */
        random_gtt_digit_from_wildcard(gtt_table, gtt_digit);
        /* Write digit to file */
        fwrite(gtt_digit, sizeof(U8bit), NUMBER_DIGIT, test);   //Write to file
    }

    fclose(test);
    free(gtt_table);
}

void random_gtt_digit_from_wildcard(GTT *gtt_table, U8bit *gtt_return)
{
    /* Random value */
    int index = rand()%NUMBER_WILDCARD;
    // printf("index = %d\n", index);
    /* Generator  */
    get_gtt_from_wildcard(gtt_table[index].gtt_digit, gtt_return);
}