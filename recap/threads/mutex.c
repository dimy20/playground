#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#define THREADS 10
#define STOVES 4

pthread_mutex_t mutex[STOVES];
int fuel[STOVES]= {100, 100, 100, 100};
void * routine(void *arg){
    for(int i = 0; i <STOVES; i++){
        if(pthread_mutex_trylock(&mutex[i]) == 0 ){
            int fuel_amount = (rand() % 30);
            if((fuel  - fuel_amount) < 0 ){
                printf("no fuel available \n");
            }else{
                fuel[i]-= fuel_amount;
                usleep(500000);
                printf("fuel left : %d \n", fuel[i]);
            }
            pthread_mutex_unlock(&mutex[i]);
            break;
        }else{
            if (i == STOVES -1){
                usleep(300000);
                i = -1;
            }
        }

    }

}
int main(){
    srand(time(NULL));
    pthread_t th[THREADS];

    for(int i =0; i<STOVES;i++){
        pthread_mutex_init(&mutex[i],NULL);
    }
    for(int i = 0; i<THREADS;i++){
        if(pthread_create(&th[i],NULL,&routine,NULL) != 0){
            perror("Failed to create thread");
        }
    }
    // clean
    for(int i = 0; i<THREADS;i++){
        if(pthread_join(th[i],NULL) != 0){
            perror("Failed to join thread");
        }
    }
    
    for(int i =0; i<STOVES;i++){
        pthread_mutex_destroy(&mutex[i]);
    }

    return 0;
}