#include "clock.h"
#include <stdio.h>
#include <unistd.h>

// 1. DEFINICIÓN DE VARIABLES GLOBALES Y COLA (Declaradas como 'extern' en clock.h)
pthread_mutex_t mutex;
pthread_cond_t cond_creator;
pthread_cond_t cond_scheduler;
pthread_cond_t cond_tick_reset; 

int time_tick = 0;
int prozesua_sortzeko_baldintza = 2;   
int schedulerra_aktibatzeko_baldintza = 5;  
int stop = 0;
int cpu_exekutatzen = 0;

Queue prozesu_ilara; 

void *clock_fun(void *arg){

    while(stop == 0){
        pthread_mutex_lock(&mutex); 

        time_tick++; 
        printf("\nTick: %d\n", time_tick);

        // Prozesu sotzaileari eta schedulerrari abisatu tick-a egin dela
        pthread_cond_broadcast(&cond_creator);
        pthread_cond_broadcast(&cond_scheduler);

        pthread_mutex_unlock(&mutex); 

        sleep(1); //segunduro tick 1
    }

    return NULL;
} 
