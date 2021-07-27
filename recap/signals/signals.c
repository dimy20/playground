#include <stdlib.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <wait.h>


#define NUM_CHILDREN 5

void send_to_parent(int write_fd, int data){
    write(write_fd, &data, sizeof(int));
};


int main(){
    int fd[NUM_CHILDREN][2];
    int id;
    for (int i = 0; i<NUM_CHILDREN;i++){
        if(pipe(fd[i]) == -1){
            printf("error creating pipe");
            exit(1);
        };
        //child here
        if(!fork()){
            // close siblings pipes
            for(int j =0; j<i;j++){
               close(fd[j][0]);
               close(fd[j][1]);
            }
            close(fd[i][0]);
            send_to_parent(fd[i][1],i);
            exit(0);
        }
    }
    int corpse, status;
    // close the write end of child's pipes
    for(int i = 0; i<NUM_CHILDREN;i++){
        close(fd[i][1]);
    }

    for(int i = 0; i<NUM_CHILDREN;i++){
        int res;
        read(fd[i][0],&res,sizeof(int));
        printf("res : %d \n",res);
    }

    for (int i = 0; i < NUM_CHILDREN; i++)
        close(fd[i][0]); 

     while ((corpse = wait(&status)) > 0)
    return 0;

}