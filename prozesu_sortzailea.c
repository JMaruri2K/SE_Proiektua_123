#include "prozesu_sortzailea.h"
#include <stdio.h>
#include <unistd.h>

void *process_creator_fun(void *arg){

    while(stop == 0){

        pthread_mutex_lock(&mutex);
        
        pthread_cond_wait(&cond_sortzailea, &mutex);

        if(stop == 1){
            pthread_mutex_unlock(&mutex);
            break;
        }

        // Prozesua sortu eta ilaran sartu
        PCB *prozesu_berria = sortu_prozesua();
        printf("Prozesu berria sortua\n");
        sartu_ilaran(&prozesu_ilara, prozesu_berria);
        fflush(stdout);

        pthread_mutex_unlock(&mutex);
    }

    return NULL;
} 
