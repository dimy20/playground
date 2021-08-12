#include <stdio.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/stat.h> 
#include <fcntl.h> 
#include <string.h>
#include <errno.h>

#define SPACE 32
#define MAX_BUFF 4096
#define WORD_SIZE_MAX 128

#define ARG_FILENAME "-f"
#define ARG_REPLACE "-r"
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

int read_file(char * filename, char * buff, size_t size){
    int fd, n_read, total_read;
    total_read = 0;
    char *p = buff;
    if((fd = open(filename,O_RDONLY)) == -1){
        return -1;
    }
    // read into buffer
    while((n_read = read(fd,p + total_read, size - total_read)) > 0){
        total_read += n_read;
    };

    (void)close(fd);
    return ((n_read < 0) ? n_read : total_read);        

}

void buff_replace_word(char * src, int size, char * word, char * new_word){
    char word_placeholder[WORD_SIZE_MAX];
    char temp_buffer[256];
    char res[MAX_BUFF];

    int j = 0;
    unsigned int r_offset = 0;
    for(int i = 0; i<size;i++){
        if(src[i] != (char)SPACE && src [i] != '\0'){
                word_placeholder[j++] = src[i];
        }else{
            if(strcmp(word_placeholder,word) == 0){
                    //printf("%s \n",src);
                    // store offset for the new world (current position - word length)
                    r_offset =  i - strlen(word_placeholder);
                    // save the adjacent data, that is from our current position on the buffer onwards
                    strcpy(temp_buffer,src + i);
                    // remove old word and adjacent data 
                    memset(src +r_offset, 0,size - i);
                    
                    src[r_offset] = '\0';
                    // concat new word
                    strncat(src,new_word,strlen(new_word));
                    // concat adjacent data
                    strncat(src,temp_buffer,sizeof(temp_buffer));

                    //printf("%s \n",src);

                    //clear temp_buffer for next iteration
                    memset(temp_buffer,0,sizeof(temp_buffer));
            }
            memset(word_placeholder,0,sizeof(word_placeholder));
            j = 0;
        }

}
}
int main(int argc, char ** argv){
    char buff[MAX_BUFF];
    memset(buff,0,MAX_BUFF);
    char * filename = argv[1];
    if(read_file(filename,buff,MAX_BUFF) == -1){
        fprintf(stderr,"Error on file : '%s', error : %s (errno : %d) \n",filename,strerror(errno),errno);
        return 1;
    };

    buff_replace_word(buff,strlen(buff),"test","computer_word");

    printf("%s \n",buff);
    return 0;

}