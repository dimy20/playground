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
/*
* Aproaches : 
    word -> string in the text file that is to be replaced
    new_word -> string the will be put in the place of word
    The text data tha does not match the word the user wants to replace, must
    remain untouched.
        ** --Example --**
            word -> line
            new_word -> newline

            original : This is a random line to illustrate
            output   : This is a random newline to illustrate
        ** --End example --**

        ** - Incorret output example -**

            word -> line
            new_word -> longer_new_line 

            original : This is a random line to illustrate
            Output   : This is a random Longer_new_lineate
            Output(Correct): This is a random Longer_new_line to illustrate

        ** - End incorret output example -**

    Scenarios:
        len(word) == len(new_word)
        len(word) > len(new_word)
        len(word) < len(new_word)
    
    Offset of adjacet text data is : len(new_word) - len(word) +1;
            
    Check if occurences is > 0:
        Copy chunks of text into a buffer

    else: return 0
    X-X-X-X-X-X-X-[W-O-R-D]X-X-X-X-X-X-X
*/

// add a reverse version? a functionallity that basically undoes the last replacement
int file_replace_word(char * filename, char * word, char * new_word){
    int w_len= strlen(word);
    int nw_len = strlen(new_word);
    int fd,bytes;
    char word_buff[256];
    char test[256];
    char tmp_buffer[256];

    memset(word_buff,0,sizeof(word_buff));
    memset(tmp_buffer,0,sizeof(tmp_buffer));
    char c;
    fd = open(filename,O_RDWR);
    int i =0;
    int count = 0;
    while(( bytes = read(fd,&c,1)) > 0){
        if(c != (char)SPACE && c != '\0'){
            word_buff[i++] = c;
        }else{
                // compare
                /*12 123 2 12 31 2 32 12 [12 32] 12 12 X X X X X  */
                if(strcmp(word_buff,word) == 0){
                    // set the proper offset
                    off_t r_offset =  -1 * (strlen(word_buff) + 1);
                    //lseek(fd,offset,SEEK_CUR);

                    // save last offset
                    off_t last_offset = lseek(fd,0,SEEK_CUR);
                    // get end offset
                    off_t end_offset  = lseek(fd,0,SEEK_END);
                    // adjacent data that is not to be modified
                    ssize_t n = end_offset - last_offset;
                    lseek(fd,last_offset-1,SEEK_SET);


                    read(fd,tmp_buffer,n);
                    //reset
                    lseek(fd,last_offset,SEEK_SET);
                    // go back len of word
                    lseek(fd,r_offset,SEEK_CUR);

                    write(fd,new_word,nw_len);
                    write(fd,tmp_buffer,sizeof(tmp_buffer));


                    lseek(fd,last_offset,SEEK_SET);
                    // save offset 
                    // add to temp buffer
                    // go back to old offser


                    count++;


/*                     read(fd,test,strlen(word_buff));
                    printf("%s \n",test); */
                }
                memset(word_buff,0,sizeof(word_buff));
                /*set i to zero so it adds new values by offset 0, 
                otherwise it will cointinue adding values from the last value of i */
                i = 0;




        }
    }
    if(bytes == -1){
        fprintf(stderr,"Error reading from file : %s (errno : %d)",strerror(errno),errno);
    }

    return count;

}
int main(){
    printf("%d", file_replace_word("demo.txt","asdasdadasdasdasdasdasdasd","x"));
    fflush(stdout);
    return 0;

}