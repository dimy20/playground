#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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
char **split(char * buff,int * count){
    int string_count = 0;

    char **res = NULL;

    // extract first word
    char * token = strtok(buff," ");
  
    while(token != NULL){
            res = realloc(res, (string_count + 1) * sizeof(*res));
            res[string_count] = malloc(STRLEN * sizeof(char));
            strcpy(res[string_count],token);  
            token = strtok(NULL," ");
            string_count++;
        }  


    *count = string_count;
    return res;
}

char ** function(char * buff){
    char ** sub_str;
      sub_str = malloc(10 * sizeof(char*));
    for (int i =0 ; i < 10; ++i)
        sub_str[i] = malloc(20 * sizeof(char));
    for (int i =0 ; i < 10; ++i){
        strcpy(sub_str[i],buff);
    }
    /* Fill the sub_str strings */
    return sub_str;
}

/* int main(){

    int count;
    char buff[256]= "hello world\0";
    char **res = split(buff,&count);

    for(int i = 0; i<count;i++){
        printf("%s ",res[i]);
    }  
    return 0;
} */
