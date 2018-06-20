#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "GTT.h"
#include "GeneratorData.h"
#include <time.h>
#include <stdlib.h>

int main(){
    srand(time(NULL));
    gen_single_gtt();
    gen_wildcard_gtt();
    return 1;
}

void gen_single_gtt()
{
    FILE *file = fopen(SINGLE_FILE, "w");
    int i;
    srand(time(NULL));
    GTT gtt_ins;
    //random data and insert to file DATA_FILE
    for(i = 0; i < NUMBER_GTT; i++){
        int j;
        for(j = 0; j < NUMBER_DIGIT - 1; j++)
        {
            int val = rand() %10;
            gtt_ins.gtt_digit[j] = val + '0';
        }
        gtt_ins.gtt_digit[NUMBER_DIGIT - 1] = '\0';
        //gen rest of gtt
        gtt_ins.dpc = rand()%65536;
        gtt_ins.ssn = rand()%128;
        gtt_ins.ri = rand()%128; 
        fwrite((const void *) &gtt_ins, sizeof(GTT), 1, file);
    }
    fclose(file);
}
//insert a char to gtt_digit
void insert_digit(U8bit *digit, int val, int *isHave)
{
    if(val < 10)
    {
        /* Insert from 0 to 9 */
        *digit = ('0' + val);
    }else {
        if(val == 10)
        {/* insert * character */
            *digit = '*';
            
        }else
        {/* Insert ? character */
            *digit = '?';
        }
        *isHave = 1;
    }
}

void gen_wildcard_gtt()
{
    FILE *file = fopen(WILDCARD_FILE, "w");     //open file
    //init var
    int i, j;
    srand(time(NULL));
    GTT gtt_ins;

    for(i = 0; i < NUMBER_WILDCARD; i++)
    {
        int length = 5 + rand()%(NUMBER_DIGIT - 5);
        int isHave = 0;
        int range = 12;
        int val;
        
        for(j= 0; j < length; j++)
        {//gen wildcard digit
            if(length == NUMBER_DIGIT - 1)
                range = 11;
            else
                range = 12;
            
            /* Select random function to generator a character or number */
            if(isHave)
            {
                val = rand()%range;
            }
            else
            {
                val = 2*(j + 1) + rand()%(range - 2 * (j + 1));
            }
            // printf("j = %d, length = %d\n", j, length);
            insert_digit( &(gtt_ins.gtt_digit[j]), val, &isHave);    //insert digit by val
        }
        gtt_ins.gtt_digit[length] = '\0';
        gtt_ins.dpc = rand()%65536;
        gtt_ins.ssn = rand()%128;
        gtt_ins.ri = rand()%128; 
        
        fwrite((const void *) &gtt_ins, sizeof(GTT), 1, file);  //Write a GTT to file
    }

    fclose(file);   //close file
}

