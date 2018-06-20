#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "MakeHtml.h"
#define MAX_LENGTH 2048

void new_file(char *title, FILE **file){
    //open file
    char file_path[MAX_LENGTH];
    sprintf(file_path, "result/%s.html", title);
    *file = fopen(file_path, "w");
    //Write start
    char start[MAX_LENGTH];
    sprintf(start, "<html>\n<head>%s</head>\n<body>\n", title);
    fwrite(start, sizeof(char), strlen(start), *file);
}

void end_file(FILE *file){
    //write end
    char *end = "\n</body>\n</html>";
    fwrite(end, sizeof(char), strlen(end), file);
    fclose(file);
}   