#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#define THREADS 2
#define MAX 100

//int arr[MAX] = {1,2,3,4,5,6,7,8,9,10};
pthread_t th[THREADS];
pthread_mutex_t lock;
pthread_cond_t cond;
int count = 0;

// prints odd number from 0-100
void *odds(void *arg){
    /*
        count % 2 == 0 --> even 
    */
    while(count < MAX){
        pthread_mutex_lock(&lock);

    // !condition
        while((count % 2 == 0))
            pthread_cond_wait(&cond,&lock);
        // do operation when the condition is true now
        printf("%d \n",count);
        count++;

        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&lock);
    }

}
void *evens(void*arg) {
    while(count < MAX){
        pthread_mutex_lock(&lock);

        while((count % 2 != 0))
            pthread_cond_wait(&cond,&lock);
        // now is even 
        printf("%d \n",count);
        count++;
        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&lock);
    }
}
int main(int argc, char ** argv){
    pthread_mutex_init(&lock,NULL);
    pthread_cond_init(&cond,NULL);
    for(int i =0;i <THREADS;i++){
        if(i == 0){
            pthread_create(&th[i],NULL,&evens,NULL);
        }else{
            pthread_create(&th[i],NULL,&odds,NULL);
        }

    }
    for(int i =0;i <THREADS;i++){
        pthread_join(th[i],NULL);
    }
    pthread_cond_destroy(&cond);
    pthread_mutex_destroy(&lock);
    return 0;
}