#ifndef ARGS
#define ARGS

#define TOO_MANY_ARGS -1 /* returned if MAX_ARGS was exceeded */

#define MAX_ARGS 128
/*Splits s into a individual strings using delim as the rule of 
  separation. s is a char* that is to be splitted, delim is a char *
  that well be used to separate each string from each other, and count
  is an int where the resulting amount of strings will be stored,
  split returns char **, containing the strings.*/
char **split(char * buff,int * count, char * delim);

int split_2(char *args[] , char * strings,const char * delim);

void split_free(char ** strings,int count);

#endif