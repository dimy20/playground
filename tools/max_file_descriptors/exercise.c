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

int write_file(char * filename, char * buff, size_t size){
    int fd, n_written;
    int total_written = 0;
    char *p = buff;
    if((fd = open(filename,O_WRONLY)) == -1){
        return -1;
    }
    while((n_written = write(fd,p + total_written ,size - total_written)) > 0){
        total_written += n_written;
    }
    (void)close(fd);
    return ((n_written < 0) ? n_written: total_written);        
}

int buff_replace_word(char * src, int size, char * word, char * new_word){
    char word_placeholder[WORD_SIZE_MAX];
    char temp_buffer[MAX_BUFF];
    unsigned int len_new_word = strlen(new_word);
    int count = 0;
    int j = 0;
    unsigned int r_offset = 0;
    for(int i = 0; i<size;i++){
        if(src[i] != (char)SPACE && src [i] != '\0'){
                word_placeholder[j++] = src[i];
        }else{
            if(strcmp(word_placeholder,word) == 0){
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

                    //clear temp_buffer for next iteration
                    memset(temp_buffer,0,strlen(temp_buffer));
                    count++;
            }
            memset(word_placeholder,0,sizeof(word_placeholder));
            j = 0;
        }
    }
    src[strlen(src) - 1] = '\0';
    return count;
}
int split_words(char * arg, char * word, char * new_word){
    int count  = 0;
    char * p =NULL;
    p = strtok(arg," ");
    strcpy(word,p);
    while(p != NULL){
        count ++;
        strcpy(new_word,p);
        p = strtok(NULL," ");
    };
    return count;
}


int main(int argc, char ** argv){
    if(argc < 2){
        fprintf(stderr,"Usage : ray [args]");
        return 1;
    }
    char f_flag = 0;
    char r_flag = 0;    
    char inv_flag = 0;    

    int option;
    char * arg = NULL;

    char word[WORD_SIZE_MAX];
    char new_word[WORD_SIZE_MAX];

    char read_buff[MAX_BUFF];
    char write_buff[MAX_BUFF];
    memset(read_buff,0,MAX_BUFF);
    memset(write_buff,0,MAX_BUFF);
    char * filename = NULL;

    /* Capture args */
    while((option = getopt(argc,argv,"f:r:")) != -1){
            switch(option){
                case 'f':
                    filename = optarg;
                    f_flag = 1;
                    break;
                case 'r':
                     arg = optarg;
                     r_flag = 1;
                     break;
                case '?':
                    fprintf(stderr,"Usage : ray [args] \n");
                    break;
                    return 1;
                default:
                    fprintf(stderr,"Usage : ray [args] \n");

            }
    }

    if(f_flag && r_flag){
        if(split_words(arg ,word,new_word) != 2){
            fprintf(stderr,"Invalid word count\n");
            return 1;
        }
    }else{
        fprintf(stderr,"Missing arguments \n");
        return 1;
    }

    if(read_file(filename,read_buff,MAX_BUFF) == -1){
        fprintf(stderr,"Error on file : '%s', error : %s (errno : %d) \n",filename,strerror(errno),errno);
        return 1;
    };
    /*Could use the same buffer, read to it from file, modify it
     and then write it back to the file but in case anyting goes wrong
     i would like to keep a copy of the original data, if an error occurs
     i just write back the origina data to the file and it would be like nothing
     ever happened.
    */
    strcpy(write_buff,read_buff);

    if(buff_replace_word(write_buff,strlen(write_buff),word,new_word) == 0){
        printf("could not find '%s' on the file, '%s' not modified.",word,filename);
        return 0;
    }


    if(write_file(filename,write_buff,MAX_BUFF) == -1){
        fprintf(stderr,"Error on file : '%s', error : %s (errno : %d) \n",filename,strerror(errno),errno);
        // write original data back
        write_file(filename,read_buff,MAX_BUFF);
        return 1;
    };
    return 0;

}