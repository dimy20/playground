#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

#define THREADS 2
#define ARR_SIZE 100000
#define RANGE 100

int arr[ARR_SIZE];

void *half_sum(void *arg){
    int sum = 0;
    int *index = (int * )arg;
    for(int i=0;i< (ARR_SIZE /2);i++){
        sum += arr[*index + i];
    }
    *(int *)arg = sum;
    return arg;
}

void init_arr(int *arr, int size, int is_range){
    for(int i =0;i<size;i++){
        int max = i+1;
        if(is_range){
            arr[i] = rand() % RANGE;
        }else{
            arr[i] = i;
        }

    };
};

int main (int argc, char* argv[]){
    int total = 0;
    srand(time(NULL));
    pthread_t th[THREADS];
    init_arr(arr,ARR_SIZE,1);


    for(int i =0; i<THREADS;i++){
        int * arg = (int *)malloc(sizeof(int));
        *arg = i * (ARR_SIZE /2);
        pthread_create(&th[i],NULL,half_sum,arg);
    }



    for(int i =0; i<THREADS;i++){
        int *r;
        pthread_join(th[i],(void **)&r);
        total += *r;
        free(r);
    }

    printf("total : %d", total);


    return 0;
}