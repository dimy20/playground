#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "args.h"
#define STRLEN 256 // since this is just for args, this is enough

int sum(int a, int b){
    return a + b;
}

void split_free(char ** strings,int count){
    for(int i = 0; i<count;i++) {
        free(strings[i]);
    }
    free(strings);
}
char **split(char * s,int * count, char * delim){
    int string_count = 0;

    char **res = NULL;

    // extract first word
    char * token = strtok(s,delim);

    while(token != NULL){
            res = realloc(res, (string_count + 1) * sizeof(*res));
            res[string_count] = malloc(STRLEN * sizeof(char));
            strcpy(res[string_count],token);  
            token = strtok(NULL," ");
            string_count++;
        }  
    
    res = realloc(res,(string_count +1 )*sizeof(*res));
    res[string_count] = NULL;

    *count = string_count;
    return res;
}

int split_2(char *args[] , char * strings,const char * delim){
    int count = 0;
    char ** next = args;
    char *token = strtok(strings,delim);

    while(token != NULL){
        *next = token;
        next++;
        token = strtok(NULL,delim);
        count++;
    }

    *next = NULL;
    return count;
}
