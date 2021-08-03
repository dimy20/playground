#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#define THREADS 2 
int mails = 0;
pthread_mutex_t mutex;

void * roll_dice(){
    int * res = (int*)malloc(sizeof(int));
    *res  = (rand() % 6)+1;
    return res;

}

void * routine (){
    for(int i =0; i<1000000;i++){
        pthread_mutex_lock(&mutex);
        mails++;
        pthread_mutex_unlock(&mutex);
    }
}
int main(int argc, char ** argv){
    int *res[THREADS];
    srand(time(NULL));
    pthread_t th[2];
    int i;
    //pthread_mutex_init(&mutex,NULL);
    for(int i =0; i<THREADS;i++){
        pthread_create(&th[i],NULL,&roll_dice,NULL);
        printf("thread has started: %d \n", i);
    }
    for(int i =0; i<THREADS;i++){
        pthread_join(*(th+i),(void**)(&res[i]));
        printf("thread has finished: %d \n", i);

    }
    for(int i =0; i<THREADS;i++){
        printf("res : %d", **(res+i));
    }

    for(int i =0; i<THREADS;i++){
        free(*(res+i));
    }
    return 0;

}