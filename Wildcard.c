#include "Wildcard.h"
#include <string.h>
#include <stdio.h>


int search_linear_wildcard(GTT *gtt_table, U8bit *gtt_digit)
{
    int i;
    for(i = 0; i < NUMBER_GTT; i++)
    {
        // printf("i = %d, gtt_digit = %p\n", i, gtt_table[i].gtt_digit);
        if(compare_with_wildcard(gtt_digit, gtt_table[i].gtt_digit) == EQUAL)
        {
            return i;
        }
    }

    return -1;
}

/* gtt_digit1 is string, gtt_digit2 is pattern */
int compare_with_wildcard(U8bit *gtt_digit1, U8bit *gtt_digit2)
{
    // printf("gtt1= %p, gtt2 = %p\n", gtt_digit1, gtt_digit2);
    //init before compare
    int n = strlen((const char *) gtt_digit1);
    int m = strlen((const char *) gtt_digit2);
    int lookup[n+1][m+1];
    memset(lookup, DIFF, sizeof(lookup));
    lookup[0][0] = SAME;
    int i, j;
    for(i = 1; i <= m; i++)
    {
        if(gtt_digit2[i-1] == '*')
        {
            lookup[0][i] = lookup[0][i-1];
        }
    }

    //compare using dynamic programming
    for(i = 1; i<=n; i++)
    {
        for(j = 1; j<=m; j++)
        {
            if(gtt_digit2[j-1] == '*')
            {//If pattern is "*" it
                lookup[i][j] = lookup[i-1][j] || lookup[i][j-1];
            }else if( (gtt_digit1[i-1] == gtt_digit2[j-1]) || (gtt_digit2[j-1] == '?') )
            {//pattern[j] equal string[i] or pattern[j] equal "?"
                lookup[i][j] = lookup[i-1][j-1];
            }else
            {//both of them is different
                lookup[i][j] = DIFF;
            }
        }
    }
    //Check value to return
    if(lookup[n][m] == SAME)
        return EQUAL;
    
    return NOT_EQUAL;
}