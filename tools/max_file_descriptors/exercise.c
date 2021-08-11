#include <stdio.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/stat.h> 
#include <fcntl.h> 
#include <string.h>
#include <errno.h>

#define SPACE 32
void clear_file(char * filename){
    fclose(fopen(filename,"w"));
}

void freplace_word(char * filename, char * word){
    int word_len = strlen(word);
    int fd,bytes;
    char word_buff[256];
    char c;
    fd = open(filename,O_RDWR);
    int i =0;
    int count = 0;
    while(( bytes = read(fd,&c,1)) > 0){
        if(c != (char)SPACE && c != '\0'){
            word_buff[i++] = c;
        }else{
                printf("%s  ",word_buff);
                fflush(stdout);
                memset(word_buff,0,sizeof(word_buff));
                i = 0;
                sleep(1);


/*             if(strcmp(word_buff,word) == 0){
                printf("bingo");
            }else{

            } */

        }
    }


}
int main(){
    char * data= "i once was a little fat boy, but now ive changed, im gay now";
    char * test = "test";

    freplace_word("file2.txt","gay");
    return 0;

}