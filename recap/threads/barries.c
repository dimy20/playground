#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#define THREADS 8


int status[THREADS] = {0};
int dice_values[THREADS];
pthread_barrier_t rolled_dice, calculated; 

void * roll(void * arg){
    int i = *(int *)arg;
    while(1){
        dice_values[i] = (rand() % 6) +1;
        pthread_barrier_wait(&rolled_dice);
        pthread_barrier_wait(&calculated);
        if(status[i] == 1){
            printf("(%d rolled %d), i won \n", i, dice_values[i]);
        }else{

            printf("(%d rolled %d), i lost \n", i, dice_values[i]);
        }
    }
    free(arg);
}
int main(int argc, char ** argv){
    srand(time(NULL));
    pthread_t th[THREADS];

    pthread_barrier_init(&rolled_dice,NULL,THREADS +1);
    pthread_barrier_init(&calculated,NULL,THREADS +1);
    for (int  i =0; i<THREADS; i++){
        int * val =(int *)malloc(sizeof(int));
        *val = i;
        if(pthread_create(&th[i],NULL,&roll,val) != 0){
            perror("failed to create the thread");
        };
    }
    // get max
    while(1){
        pthread_barrier_wait(&rolled_dice);
        int max = 0;
        for(int i =0; i<THREADS; i++){
            if(dice_values[i]> max){
                max = dice_values[i];
            }
        };
        for(int i =0; i<THREADS;i++){
            if(dice_values[i] == max){
                status[i] = 1;
            }else{
                status[i] = 0;
            }
        }
        sleep(1);
        printf("==== New round starting ====\n");
        pthread_barrier_wait(&calculated);
    }


    for (int  i =0; i<THREADS; i++){
        if(pthread_join(th[i],NULL) != 0){
            perror("failed to join the thread");
        }
    }
    pthread_barrier_destroy(&rolled_dice);
    pthread_barrier_destroy(&calculated);
    return 0;

}