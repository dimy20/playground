#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sysexits.h>
#include <unistd.h>
#include <signal.h>

#include "args.h"

#define BUFF_SIZE 256
void debug_s(char *s, int size){
    for(int i = 0 ;i <size;i++){
        printf("%d ",s[i]);
    }

}
void signal_handler(int s){
    (void)s; // shhh
    int errno_aux = errno;
    while(waitpid(-1,NULL,WNOHANG) > 0);
    errno = errno_aux;
    printf("<$shell$> ");
    fflush(stdout);
}

char *get_string(char * buff, size_t buflen){
    printf("<$shell$> ");
    return fgets(buff,buflen,stdin);
}



int main(int argc, char ** argv){
    const char * delim = " ";
    char buff[BUFF_SIZE];
    memset(buff,0,BUFF_SIZE);
    int pid,count;
    char ** res = NULL;
    struct sigaction sa;
    memset(&sa,0,sizeof((sa)));
    sa.sa_handler = &signal_handler;
    sa.sa_flags = SA_RESTART;   

    if(sigaction(SIGCHLD,&sa,NULL) == -1){
        fprintf(stderr,"cant set sigaction to handle SIGCHLD signal: %d \n",errno);
    }

    char *args[MAX_ARGS];
/*     char test[256] = "Hello world this is a test";
    int  n = split_2(args,test," "); */

/*     for(int i =0; i<n;i++){
        printf("%s \n",args[i]);
    } */

    while(get_string(buff,BUFF_SIZE) != NULL){
        buff[strlen(buff) -1 ] = '\0';
/*         res = split(buff,&count," "); */
        split_2(args,buff,delim);

        if((pid = fork()) == -1){
            fprintf(stderr,"Can't fork : %s \n",strerror(errno));
        }
        else if(pid == 0){
            if(execvp(args[0],args) == -1){
                fprintf(stderr,"Can't execute command %s: %s \n",buff,strerror(errno));
            }
            exit(EX_UNAVAILABLE);
        }


        memset(buff,0,sizeof(buff));
/*         split_free(res,count); */
    }

    return 0;
}