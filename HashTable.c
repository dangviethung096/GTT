#include <stdio.h>
#include <string.h>
#include "LinkedList.h"
#include "GTT.h"
#include "HashTable.h"
#include "MakeHtml.h"


int hash(U8bit *gtt_digit){
    int length = strlen((const char *) gtt_digit);
    int sum = 0;
    int i;
    for(i = 0; i < length; i++){
        int num = gtt_digit[i] - 48;
        sum += (i+1)*(num * num);
    }
    return sum % TABLE_LENGTH;
}

int search_key(hash_table *table, U8bit *gtt_digit)
{
    int hash_code = hash(gtt_digit);
    hash_table bucket = (hash_table) search_gtt_digit((linked_list_gtt *) table[hash_code], gtt_digit);
    if(bucket != NULL)
        return bucket->index_in_table;

    //If cannot find element in bucket this return -1
    return -1;
}

void add_bucket(hash_table *table, U8bit *gtt_digit, int index_in_table)
{
    int hash_code = hash(gtt_digit);
    if(table[hash_code] ==  NULL)
        insert_head(table + hash_code, gtt_digit, index_in_table);
    else
        push_back_gtt(table[hash_code], gtt_digit, index_in_table);
}

void write_table(hash_table *table, char *file_name)
{
    int i, max = 0;
    FILE *file;
    new_file(file_name, &file);
    fwrite("<table border=\"1px solid black\">\n", strlen("<table border=\"1px solid black\">\n"), sizeof(char), file);
    for(i = 0; i < TABLE_LENGTH; i++){
        fwrite("<tr>\n", strlen("<tr>\n"), sizeof(char), file);
        
        char head[1024]; 
        sprintf(head, "<td>Hash code: %d -></td>\n", i);
        fwrite(head, strlen(head), sizeof(char), file);

        hash_table loop_val = table[i];
        while(loop_val != NULL){
            char val[1024];
            sprintf(val, "<td>%s -></td>\n", loop_val->gtt_digit);
            fwrite(val, strlen(val), sizeof(char), file);
            loop_val = loop_val->next_gtt;
        }
        
        fwrite("</tr>\n", strlen("</tr>\n"), sizeof(char), file);
        if(max < size_list(table[i])) 
            max = size_list(table[i]);
    }
    fwrite("</table>\n", strlen("</table>\n"), sizeof(char), file);
    char message[2048];
    sprintf(message, "<p>Do dai lon nhat trong danh sach: %d</p>\n", max);
    fwrite(message, strlen(message), sizeof(char), file);
    end_file(file);
}

void delete_table(hash_table *table){
    int i;
    for(i = 0; i < TABLE_LENGTH; i++){
        collect_garbage(table[i]);
    }
}