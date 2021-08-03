#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <semaphore.h>
#include <unistd.h>

// problems
/*
    1) Manage shared memory accessed by many threads, securing that
        no race conditions occure.
    2) #Producers > #Consumers :
           This means the producer will be adding more items to the
           buffer than the consumer can handle, at one point the buffer
           will be filled, we need to take this into account.
    2) #Producers < #Consumers :
            In this case, the rate of producing items is slower 
            compared to the rate of consumption,so it may occur that
            the consumer might try to read from an empty buffer,
            what should the behaviour be on this event?

    In this simple approach, producers will producer simple random
    integers, and consumers will proccess this data by printing them
    on the terminal.
*/


//#define THREADS 8
#define BUFFER_SIZE 10


#define ARG_PRODUCER "-p"
#define ARG_CONSUMER "-c"
#define ARG_HELP "--help"

//buffer that  producer and consumer write and read to respectively.
int buffer[BUFFER_SIZE];
int count = 0;

pthread_mutex_t mutex_buff;
sem_t sem_available_slots , sem_used_slot;
void * producer(void *args){
    while(1){

        // decrement 1 if sem_available_slots > 0 else : block
        sem_wait(&sem_available_slots);
        // get a lock
        pthread_mutex_lock(&mutex_buff);
        //random number selected from (0,99);
            buffer[count++] = rand() % 100;
        pthread_mutex_unlock(&mutex_buff);
        // increment 1 used slot
        sem_post(&sem_used_slot);

    }
}
void * consumer(void *args){

    while(1){
        int i = -1;

        // if sem_used_slots > 0 then decrement 1 else : block
        sem_wait(&sem_used_slot);
        pthread_mutex_lock(&mutex_buff);
            i = buffer[count-1];
            count--;
        // after removing we have one available slot again so increment 1
        sem_post(&sem_available_slots);
        pthread_mutex_unlock(&mutex_buff);

        // consume
        printf("got %d \n",i);
        sleep(1);
    }

}

int help(){
    printf("Usage: \n");
    printf("main [options] \n");
    printf("Options: \n");
    printf("  "ARG_CONSUMER" int  specify number of consumers \n");
    printf("  "ARG_PRODUCER" int  specify number of producers \n");
    return 0;
}
int no_args(){
        printf("usage : main [options] \n");
        printf("type --help \n");
        return 0;
}
int main(int argc, char ** argv){

    if(argc < 2){
        return no_args();
    }
    int prod_n, cons_n;
    // get args
        for(int i = 0 ; i<argc;i++){

            if(strcmp(argv[i],ARG_HELP) == 0 ){
                return help();
            }
            if(strcmp(argv[i],ARG_PRODUCER) == 0){
                if((prod_n= atoi(argv[i+1])) == 0){
                    printf("invalid argument \n");
                    return 0;
                }
            }
            if(strcmp(argv[i],ARG_CONSUMER) == 0){
                if((cons_n= atoi(argv[i+1])) == 0){
                    printf("invalid argument \n");
                    return 0;
                }
            }
        }
    int THREADS = prod_n + cons_n;
    pthread_t th[THREADS];
    printf("starting %d threads : (%d producers, %d consumers) \n",THREADS, prod_n,cons_n);

    memset(buffer,0,sizeof(buffer));
    srand(time(NULL));
    pthread_mutex_init(&mutex_buff,NULL);
    sem_init(&sem_available_slots,0,THREADS);
    sem_init(&sem_used_slot,0,0);

    //create producer threads
    for(int i = 0 ; i<prod_n;i++){
        if(pthread_create(&th[i],NULL,&producer,NULL) != 0){
            perror("Failed to create thread");
        }
    }
    for(int i = 0 ; i<cons_n;i++){
            if(pthread_create(&th[i],NULL,&consumer,NULL) != 0){
                perror("Failed to create thread");
            }
        }

    for(int i =0; i<THREADS;i++){
        if(pthread_join(th[i],NULL) != 0){
                perror("Failed to join the thread");
        }
    }
    sem_destroy(&sem_available_slots);
    sem_destroy(&sem_used_slot);
    pthread_mutex_destroy(&mutex_buff);
    return 0;
}