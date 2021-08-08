#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define CHUNK 2
#define STRLEN 256 // since this is just for args, this is enough

int split(char * buff){
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


    for(int i = 0; i<string_count;i++){
        printf("%s ",res[i]);
    }  
    for(int i = 0; i<string_count;i++) {
        free(res[i]);
    }
    free(res);
    return string_count;
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
/*     char buff[256];
    memset(buff,0,sizeof(buff));
    read_s(buff,sizeof(buff));
    buff[strlen(buff)-1] = '\0'; */
    char buff[256]= "hello world\0";
    split(buff);



    return 0;
}
