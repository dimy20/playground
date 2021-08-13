#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <getopt.h>
#define BUFF_SIZE 32767 

int read_file(int fd, char * buff, size_t size){
    int n_read;
    int total_read = 0;
    while((n_read = read(fd,buff + total_read,size-total_read)) > 0 ){
            total_read += n_read;
    }
    
    return n_read < 0 ? n_read : total_read;
}
int write_file(int fd, char * buff, size_t size){
    int n_read;
    int total_read = 0;
    while((n_read = write(fd,buff + total_read,size - total_read)) > 0 ){
            total_read += n_read;
    }
    return n_read < 0 ? n_read : total_read;
}

int main(int argc, char ** argv){
    if(argc < 3){
       fprintf(stderr,"Usage : '%s' <target> <source>\n",argv[0]);
       return 1;
    }

    char buff[BUFF_SIZE];
    memset(buff,0,sizeof(buff));
    char * src, * target;
    int fd;
    struct stat  s;


    src = argv[1];
    target= argv[2];

    if((fd = open(src,O_RDONLY)) == -1){
       fprintf(stderr,"Error opening file '%s': %s\n",src,strerror(errno));
       return 1;
    }
    if(read_file(fd,buff,BUFF_SIZE) == -1){
       fprintf(stderr,"Error readin from file '%s' : %s\n",src,strerror(errno));
       return 1;
    }
    (void)close(fd);

    if((fd = open(target,O_CREAT | O_WRONLY)) == -1){
        if(errno == EISDIR){
            strncat(target,"/",2);
            strncat(target,src,strlen(src));
        }else{
            fprintf(stderr,"Could not create target file '%s' : %s\n",target,strerror(errno));
            return 1;
        }
    }
    printf("%s \n",target);
    if((fd = open(target,O_CREAT | O_WRONLY)) == -1){
       fprintf(stderr,"Could not create target file '%s' : %s\n",target,strerror(errno));
       return 1;
    }

    if(write(fd,buff,strlen(buff)) == -1){
        fprintf(stderr,"Error writing to target file '%s' : %s\n",target,strerror(errno));
        return 1;
    }

    return 0;




}