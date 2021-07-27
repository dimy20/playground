#include <stdio.h>
#include <unistd.h>
#include <wait.h>
#include <signal.h>
#include <string.h>


void sig_handler (int s){
    (void)s;
    while(waitpid(-1,NULL,WNOHANG) > 0);
};

int main(){
    int id;
    int child_pid;
    int fd[2];

    struct sigaction sa;

    memset(&sa,0,sizeof(sa));
    sa.sa_handler = sig_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;

    if(pipe(fd) == -1) {
        printf("failed to create the pipe");
        return 1;
    }

    if(id = !fork()){
        close(fd[0]);
        int pid = getpid();
        write(fd[1],&pid,sizeof(int));
        close(fd[1]);
        return 0 ;
    }
    if(id == -1){
        printf("failed to fork");
        return 1;
    }


    close(fd[1]);
    read(fd[0],&child_pid,sizeof(int));
    close(fd[0]);

    printf("child pid is  : %d", child_pid);
    fflush(stdout);
    return 0;
}