#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define STRLEN 256 // since this is just for args, this is enough

char **split(char * buff,int * count){
    int string_count = 0;
    char **res;
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
void split_free(char ** strings,int count){
    for(int i = 0; i<count;i++) {
        free(strings[i]);
    }
    free(strings);
}
void debug(char *s, int size){
    printf("len : %ld \n", strlen(s));
    printf("values :");
    for(int i=0;i<size;i++){
        printf("%d ",s[i]);
    }
        printf("\n");
}
void read_s(char * buff, int size){
    printf("type : ");
    fgets(buff,size,stdin);
}
int main(){

    int count;
    char buff[256]= "hello world\0";
    char **res = split(buff,&count);
    split_free(res,count);

    for(int i = 0; i<count;i++){
        printf("%s ",res[i]);
    }  


    return 0;
}
