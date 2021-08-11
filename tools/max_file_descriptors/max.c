
#include <sys/resource.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void open_n_files(int n){
    struct stat stats;
    int count = 0;
    int fd;
    for(int i=0;i<n;i++){
        if(fstat(i,&stats) == 0){
            printf("Fd %d is open \n",i);
            count++;
        }

    }
    for(int i =0; i< n+1;i++){
        if((fd = open("/dev/null",O_RDONLY)) < 0) {
            if(errno == EMFILE){

                fprintf(stderr,"Opened %d additional files, then failed : %s (%d)",i-count,strerror(errno),errno);
                break;
            }else{
                fprintf(stderr, "Cant open '/dev/null' on fd %d : %s (errno %d)",i,strerror(errno),errno);
                break;
}
        }
        
    }

    printf("Open files : %d \n",count);
}
int main(){
    struct rlimit rlm;
    int openmax;
    #ifdef OPEN_MAX
        printf("macro OPEN_MAX is defined as %s \n",OPEN_MAX);
    #else
        printf("macro OPEN_MAX is not defined on the system .\n");
    #endif
    printf("OPEN MAX IS (getdtablesize) : %d \n", getdtablesize());   
    printf("getconf says : ");
    fflush(stdout);
    (void)system("getconf OPEN_MAX");
    errno = 0;
    if((openmax = sysconf(_SC_OPEN_MAX)) < 0){
        if(errno == 0){
            fprintf(stderr,"This limit is indeterminate \n");
        }
        if(errno == -1){
            fprintf(stderr,"error for SC_OPEN_MAX : %s \n", strerror(errno));
        }
        return 1;
    }
    printf("sysconf says this process can open %d files \n",openmax);
    if(getrlimit(RLIMIT_NOFILE,&rlm) != 0){
        fprintf(stderr,"Error getrlimit with RLIMIT_NOFILE : %s",strerror(errno));
        return 1;
    }
    openmax = (int)rlm.rlim_max;
    printf("getrlimit(2) says this process can open %d files.\n", openmax);
    open_n_files(openmax);  

    return 0;

}

