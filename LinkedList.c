#include <stdio.h>
#include <stdlib.h>
#include "LinkedList.h"
#include <string.h>
//Search element before element that have same gtt_digit
linked_list_gtt *search_before(linked_list_gtt *gtt_list, U8bit *gtt_digit){
    U8bit *next_gtt_digit;
    if(gtt_list == NULL) return NULL;
    while(gtt_list->next_gtt != NULL){
        next_gtt_digit = gtt_list->next_gtt->gtt_digit;
        if(compare_gtt_digit(next_gtt_digit, gtt_digit) == EQUAL){
            return gtt_list;
        }
        gtt_list = gtt_list->next_gtt;
    }
    return NULL;
}
//Search element that have same gtt_digit
linked_list_gtt *search_gtt_digit(linked_list_gtt *gtt_list, U8bit *gtt_digit){
    
    while(gtt_list != NULL){
        if(compare_gtt_digit(gtt_list->gtt_digit, gtt_digit) == EQUAL){
            return gtt_list;
        }
        gtt_list = gtt_list->next_gtt;
    }
    return (linked_list_gtt *) NULL;
}

void push_back_gtt(linked_list_gtt *gtt_list, U8bit *gtt_digit, int index_in_table)
{
    //GO TO END LIST
    if(gtt_list != NULL){
        while(gtt_list->next_gtt != NULL){
            gtt_list = gtt_list->next_gtt;
        }
        //allocate new memory for gtt
        gtt_list->next_gtt = malloc(sizeof(linked_list_gtt));
        gtt_list = gtt_list->next_gtt;
        strcpy( (char *)gtt_list->gtt_digit, (char *)gtt_digit);
        gtt_list->index_in_table = index_in_table;
        gtt_list->next_gtt = NULL;
    }
}

void insert_head(linked_list_gtt **gtt_list, U8bit *gtt_digit, int index_in_table)
{
    linked_list_gtt *after = *gtt_list;
    *gtt_list =(linked_list_gtt *) malloc(sizeof(linked_list_gtt));
    strcpy( (char *) (*gtt_list)->gtt_digit,(char *) gtt_digit);
    (*gtt_list)->index_in_table = index_in_table;
    (*gtt_list)->next_gtt = after;
}

int size_list(linked_list_gtt *gtt_list){
    int count = 0;
    while(gtt_list != NULL){
        gtt_list = gtt_list->next_gtt;
        count++;
    }
    return count;
}
//Free all mem for list parameter
void collect_garbage(linked_list_gtt *gtt_list){
    linked_list_gtt *removed;
    while(gtt_list != NULL){
        removed = gtt_list;
        gtt_list = gtt_list->next_gtt;
        free(removed);
    }
}

linked_list_gtt delete();