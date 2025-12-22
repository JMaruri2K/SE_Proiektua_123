#include "scheduler.h"
#include <stdio.h>
#include <unistd.h>

void *scheduler_aktibatzea(void *arg){

    PCB *prozesua_exekutatu = NULL;
    int azken_ticka_sched = 0;

    while(stop == 0){

        pthread_mutex_lock(&mutex);
        
        if (time_tick % schedulerra_aktibatzeko_baldintza == 0 && azken_ticka_sched != time_tick && cpu_exekutatzen == 1){

            printf("ADI, CPU-a oraindiki okupatuta dago beste prozesu bat exkutatzen.\n ");

        }

        // Esperar hasta que el tick alcance el umbral del scheduler (scheduler_threshold)
        while((time_tick % schedulerra_aktibatzeko_baldintza != 0 || azken_ticka_sched == time_tick || cpu_exekutatzen == 1) && stop == 0){ 
            pthread_cond_wait(&cond_scheduler, &mutex);
        }

        if(stop == 1){
            pthread_mutex_unlock(&mutex);
            break;
        }

        // Schedulerr politika aktibatzea
        printf("\nSchedulerra abiatzen\n");
        
        // Ze prozesu aterako den ilaratik
        prozesua_exekutatu = dado_txapelketa(&prozesu_ilara);
        
        if (prozesua_exekutatu != NULL) {
            //printf("Prozesua ilaratik atera da exekutatzeko: %d. Ilara geratzen den prozesu kopurua: %d\n", prozesua_exekutatu->pid, prozesu_ilara.count);

            printf("Ilara geratzen den prozesu kopurua: %d\n", prozesu_ilara.count);
            
            cpu_exekutatzen = 1; // CPU okupatu

            //Hari bat sortu prozesuaren exekuzioa simulatzeko.
            pthread_t thread_exekutatzen;
            //Exekuzioa simulatu.
            if(pthread_create(&thread_exekutatzen, NULL, (void *)exekutatu_prozesua, (void *)prozesua_exekutatu) == 0){
                pthread_detach(thread_exekutatzen);
            }else{
                //Haria sortzean errorea badago, prozesua libre utzi eta CPU libre utzi.
                free(prozesua_exekutatu);
                cpu_exekutatzen = 0; // CPU libre utzi
            }

        } else {
            printf("Ilara hutsik dago. Ez dago prozesurik exekutatzeko.\n");
        }

        azken_ticka_sched = time_tick;
        
        pthread_mutex_unlock(&mutex);
    }

    return NULL;
} 

void *exekutatu_prozesua(PCB *arg){

    PCB *prozesua = (PCB*)arg;
    int iraupena = (rand() % 7) + 1; // 1-7 segunduko iraupena


    printf("Prozesua exekutatzen: %d\n", prozesua->pid);
    printf("Prozesua exekutatzeko iraupena:  %d\n" , iraupena);

    sleep(iraupena); // Simulamos el tiempo de ejecución

    pthread_mutex_lock(&mutex);
    cpu_exekutatzen = 0; // CPU libre utzi

    printf("Prozesua amaitu da: %d\n", prozesua->pid);
    pthread_mutex_unlock(&mutex);

    free(prozesua);
    return NULL;
}
