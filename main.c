#include <stdio.h>
#include <unistd.h>



void write_to_console(char * buff, int size){
    for(int i =0; i<size; i++){
        fprintf(stdout,"%c",buff[i]);
    };
}


int main(){
    write_to_console("test",4);
    return 0;
}
