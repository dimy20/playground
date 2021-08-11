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

        ** - End incorret output example -**
        
    Scenarios:
        len(word) == len(new_word)
        len(word) > len(new_word)
        len(word) < len(new_word)
    X-X-X-X-X-X-X-[W-O-R-D]X-X-X-X-X-X-X
*/
int freplace_word(char * filename, char * word, char * new_word){
    int w_len= strlen(word);
    int nw_len = strlen(new_word);
    int fd,bytes;
    char word_buff[256];
    char test[32];

    memset(word_buff,0,sizeof(word_buff));
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
                    off_t offset =  -1 * (strlen(word_buff) + 1);
                    lseek(fd,offset,SEEK_CUR);
                    write(fd,new_word,nw_len);

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
    char * data= "i once was a little fat boy, but now ive changed, im gay now";
    char c = 0;

/*     int fd = open("file2.txt",O_RDWR);
    lseek(fd,(off_t)10,SEEK_SET);
    read(fd,&c,1);
    printf("%c",c);
    fflush(stdout);
 */


    printf("%d", freplace_word("file2.txt","teta","tetasss"));
    fflush(stdout);
    return 0;

}