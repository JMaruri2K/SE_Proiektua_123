#include "scheduler.h"
#include <stdio.h>
#include <unistd.h>

void *scheduler_aktibatzea(void *arg){

    PCB *prozesua_exekutatu = NULL;

    while(stop == 0){

        pthread_mutex_lock(&mutex);
        
        pthread_cond_wait(&cond_scheduler, &mutex);

        if(stop == 1){
            pthread_mutex_unlock(&mutex);
            break;
        }

        // Scheduler politika aktibatzea
        printf("\nSchedulerra abiatzen\n");
        
        if(cpu_exekutatzen == 0 && prozesu_ilara.count > 0){

            // Ze prozesu aterako den ilaratik
            prozesua_exekutatu = zozketa(&prozesu_ilara);  

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

        }
        
        pthread_mutex_unlock(&mutex);

    }

    return NULL;
}

void *exekutatu_prozesua(PCB *arg){

    PCB *prozesua = (PCB*)arg;

    printf("Prozesua exekutatzen: %d\n", prozesua->pid);
    printf("Prozesua exekutatzeko iraupena:  %d\n" , prozesua->exekuzio_denbora);

    sleep(prozesua->exekuzio_denbora);


    pthread_mutex_lock(&mutex);

    printf("Prozesua amaitu da: %d\n", prozesua->pid);
    free(prozesua);


    cpu_exekutatzen = 0; // CPU libre utzi
    
    pthread_mutex_unlock(&mutex);

    return NULL;
}
