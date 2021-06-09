#include <stdio.h>
#include <stdlib.h>
int * copy(int *A, int size){
    int *end = A+size;
    int *res = malloc(sizeof(int) *size);
    int *first = A;
    while(A < end){
        res[A-first] = *A;
        A++;
    };
    return res;
};

int sumElems(const int *A,int size){
    int sum = 0;
    for(int i =0; i<size;i++) {
        sum = A[i] + sum;
    }
   return sum;
}

char *get_string(FILE * stream){
    int bytes = 0;
    char *buff = NULL;
    int c;
    while((c=fgetc(stream) != EOF && c != '\r' && c!= '\n')){
        bytes++;
        buff = realloc(buff,bytes+1);
        if(buff == NULL){
            return NULL;
        }
        buff[bytes-1] = c;
    }
    if(c == '\r'){
        c = fgetc(stream);
        if(c != '\n'){
            ungetc(c,stream);
        }
    }
    // means there was nothing to read
    if(buff == NULL){
        if(c == EOF){
            return NULL;
        }else{
            buff = malloc(1);
        }

    }
    buff[bytes] = 0;
    return buff;

}
char * get_string_(FILE * stream){
    int capacity = 20;
    char * buff = malloc(capacity);
    int size = 0;
    int c;
    while(1){
        size++;
        //buff = realloc(buff,1);
        if(size >= capacity){
            capacity = capacity * 2;
            buff = realloc(buff,capacity);
            if(buff == NULL) break;
        }

        c = fgetc(stream);
        buff[size-1] = c;

        if(feof(stream)){
            break;
        }
    }
    printf("%d",size);

    if(buff == NULL){
        if (feof(stream)) return NULL;
    }
    buff[size-1] = 0;
    return buff;

}

int main(){
    FILE * file = fopen("./test.txt","r");
    if(file == NULL){
        perror("failed to open file");
        return -1;
    }else{
        char *string = get_string_(file);
        fclose(file);
        printf("%ld",sizeof(string));
        printf("%s",string);
    }



    /*
    char *str;
    while((str = get_string(fin)) != NULL){
        printf("%s",str);
    }
*/
    return 0;
}
