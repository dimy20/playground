#include <stdio.h>
#include <unistd.h>
#include <wait.h>

int main(){
    int id1 = fork();
    if(id1 == 0){
        printf("hello world process 1 \n");
        sleep(1);
        printf("finished 1 %d\n",getpid());
        return 0;
        //execlp("ls","ls","-al",NULL);
    }
    if(id1 == -1){
        printf("Error creating process");
        return 1;
    }

    int id2 = fork();
    if(id2 == 0){
        printf("hello world process 2 \n");
        sleep(3);
        printf("finished 2 %d\n",getpid());
        return 0;
        //execlp("ls","ls","-al",NULL);
    }
    if(id2 == -1){
        printf("Error creating process");
        return 1;
    }
    int res1 = wait(NULL);
    printf("%d \n",res1);
    int res2 = wait(NULL);
    printf("%d \n",res2);
    return 0;
};
