#include "GTT.h"
#include <string.h>

int compare_gtt_digit(U8bit *digit_1, U8bit *digit_2){
    int i;
    int flag = EQUAL;
    int length = strlen( (const char *)digit_1);
    if(length != strlen( (const char *)digit_2))
        return NOT_EQUAL;
    //Caution: this function also check end of string '\0'.
    for(i = 0; i <= length; i++){
        if(digit_1[i] != digit_2[i]){
            flag = NOT_EQUAL;
            break;
        }
    }
    return flag;
}