#include "clock.h"
#include <stdio.h>
#include <unistd.h>

void *clock_fun(void *arg){

    while(stop == 0){
        pthread_mutex_lock(&mutex); 

        time_tick++; 
        printf("\nTick: %d\n", time_tick);

        // Prozesu sotzaileari eta schedulerrari abisatu tick-a egin dela
        pthread_cond_broadcast(&cond_timer);

        pthread_mutex_unlock(&mutex); 

        sleep(1); //segunduro tick 1
    }

    return NULL;
} 
