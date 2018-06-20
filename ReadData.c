#include <stdio.h>
#include <stdlib.h>
#include "ReadData.h"

void read_data(GTT **gtt_table, char *data_file)
{
    FILE *file = fopen(data_file, "r");
    *gtt_table =(GTT *) calloc(sizeof(GTT), NUMBER_GTT);
    fread(*gtt_table, sizeof(GTT), NUMBER_GTT, file);
    fclose(file);
}


void read_test(U8bit ***test_set ,char *test_file)
{
    /* allocate memory to test_set pointer */
    (*test_set) = (U8bit **) calloc(NUMBER_TEST, sizeof(U8bit *));
    int i;
    FILE *test = fopen(test_file, "r");
    for(i = 0; i < NUMBER_TEST; i++)
    {//allocate memory and read from file
        (*test_set)[i] = (U8bit *) calloc(NUMBER_DIGIT, sizeof(U8bit));
        fread((*test_set)[i], sizeof(char), NUMBER_DIGIT, test);
    }
    fclose(test);
}
