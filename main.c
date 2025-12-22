#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>

// Funtzio guztiak importatu
#include "pcb_ilara.h"        
#include "clock.h"            
#include "prozesu_sortzailea.h" 
#include "scheduler.h"        

int main(){
    
    pthread_t tid_clock, tid_creator, tid_scheduler; 
    
    // exekuzio-denbora segundutan
    const int execution_time = 10;

    // Hasieratu ahauzazko zenbakiak sortzeko pid-entzat
    srand(time(NULL)); 

    // Ilara hasieratu
    hasieratu_Queue(&prozesu_ilara);
    
    // clock-ean erabiliko diren gauzak hasieratu
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond_creator, NULL);
    pthread_cond_init(&cond_scheduler, NULL);
    pthread_cond_init(&cond_tick_reset, NULL); 
    
    // Hariak sortu
    printf("Clock-a abiarazten, Prozesu Sortzailea %ds-ro y Schedulerra %ds-ro. Exekutatzen %d segunduz...\n", prozesua_sortzeko_baldintza, schedulerra_aktibatzeko_baldintza, execution_time);
    pthread_create(&tid_clock, NULL, clock_fun, NULL);
    pthread_create(&tid_creator, NULL, process_creator_fun, NULL);
    pthread_create(&tid_scheduler, NULL, scheduler_aktibatzea, NULL);

    // Ejecución principal (el sistema funciona en segundo plano)
    sleep(execution_time); 
    
    // mutexa blokeatu eta aktibatu bukatzeko flag-a
    pthread_mutex_lock(&mutex); 
    stop = 1;
    
    // Hari guztiak esnatu ikusteko stop-a = 1 dela eta bukatu egin behar dutela.
    pthread_cond_broadcast(&cond_creator);
    pthread_cond_broadcast(&cond_scheduler);
    pthread_cond_broadcast(&cond_tick_reset); 
    pthread_mutex_unlock(&mutex);

    // Hari guztiak bukatzea itxaron
    pthread_join(tid_clock, NULL);
    pthread_join(tid_creator, NULL);
    pthread_join(tid_scheduler, NULL);
    printf("\n[MAIN] Hariak geldituta\n");

    // Ilara garbitu exekutatu gabe gelditu diren prozesuetaz
    pthread_mutex_lock(&mutex); 
    ilara_garbitu(&prozesu_ilara);
    pthread_mutex_unlock(&mutex);
    
    // mutex-a eta kondizioak suntzitu
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond_creator);
    pthread_cond_destroy(&cond_scheduler);
    pthread_cond_destroy(&cond_tick_reset); 
    printf("[MAIN] Mutex eta kondizioak suntzituta. Programa bukatuta.\n");

    return 0;
} 
