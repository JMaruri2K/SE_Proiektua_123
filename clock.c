#include "clock.h"
#include <stdio.h>
#include <unistd.h>

void *clock_fun(void *arg){

    while(stop == 0){

        sleep(1); //segunduro tick 1

        if(stop == 1){
            break;
        }

        pthread_mutex_lock(&mutex); 

        time_tick++; 
        printf("\nTick: %d\n", time_tick);

        // Timer-ari abisatu tick-a egin dela
        pthread_cond_broadcast(&cond_timer);

        pthread_mutex_unlock(&mutex); 

    } 
    return NULL;
} 
