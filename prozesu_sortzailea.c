#include "prozesu_sortzailea.h"
#include <stdio.h>
#include <unistd.h>

void *process_creator_fun(void *arg){

    int azken_ticka = 0;

    while(stop == 0){

        pthread_mutex_lock(&mutex);
        
        // Itxaron tick-a sortze-atalasearen multiploa izan arte
        while((time_tick % prozesua_sortzeko_baldintza != 0 || azken_ticka == time_tick)  && stop == 0){ 
            pthread_cond_wait(&cond_creator, &mutex);
        }

        if(stop == 1){
            pthread_mutex_unlock(&mutex);
            break;
        }

        // Prozesua sortu eta ilaran sartu
        PCB *prozesu_berria = sortu_prozesua();
        printf("Prozesu berria sortua\n");
        sartu_ilaran(&prozesu_ilara, prozesu_berria);
        fflush(stdout);

        azken_ticka = time_tick;

        pthread_mutex_unlock(&mutex);
    }

    return NULL;
} 
