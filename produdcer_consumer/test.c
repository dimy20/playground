#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#define THREADS 10

int arr[THREADS] = {1,2,3,4,5,6,7,8,9,10};
pthread_t th[THREADS];
pthread_cond_t cond;
pthread_mutex_t mutex;
int count = 0;

void*func(void*arg){

    pthread_mutex_lock(&mutex);
    int index  = *(int*)arg;
    printf("(index : %d, count : %d) \n",index,count);
    while(1){
        if(count != index){
            pthread_cond_wait(&cond,&mutex);
            count++;
        }else{
            pthread_cond_signal(&cond);
            printf("%d\n", arr[index]);
        }
    }
    pthread_mutex_unlock(&mutex);
    free(arg);
}

int main(int argc, char **argv){
    pthread_mutex_init(&mutex,NULL);
    pthread_cond_init(&cond,NULL);
    for(int i=0;i<THREADS;i++){
        int * n = malloc(sizeof(int));
        *n = i;
        if(pthread_create(&th[i],NULL,&func,n) != 0){
            perror("error creating thread");
        }
    }
    for(int i=0;i<THREADS;i++){
        if(pthread_join(th[i],NULL) != 0){
            perror("error joining thread");
        }
    }
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);
    return 0;

}