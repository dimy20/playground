#include<stdio.h>
#include<pthread.h>
#include <unistd.h>

typedef struct {
    char buff[2];
} buff_sample_t;

typedef struct {
    char first;
    char second;
    char third;
    short fourth;
} sample_t;

void mem_layout(sample_t * var){
    //printf("%li",sizeof(*var));
    printf("memory layout of struct : ");
    unsigned char * ptr = (unsigned char *)var;
    for(int i=0; i<sizeof(*var);i++,ptr++){
        printf("%d ",(unsigned int)*ptr);
    }
    printf("\n");
     
}

int main(int argc, char ** argv){
    int test = 43;
    char buff[4];
    sample_t var;
    var.first = 'A';
    var.second = 'B';
    var.third = 'C';
    var.fourth = 123;
    
    // this doesnt work : buff = (char *)&test; 
    
    buff_sample_t * var_arr = (buff_sample_t *)&test;
    for(int i =0 ; i<4;i++){printf("%d ", var_arr->buff[i]);};
    int res = (var_arr->buff[0] << 8) | var_arr->buff[1];
    printf("res : %d",res);
    
    /*

    printf("%d",var_arr->buff[0]);
    */
    return 0;
}

