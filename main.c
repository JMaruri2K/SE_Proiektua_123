#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>

// Funtzio guztiak importatu
#include "pcb_ilara.h"      
#include "timer.h"  
#include "clock.h"            
#include "prozesu_sortzailea.h" 
#include "scheduler.h"        

// Aldagaien definizio globala
pthread_mutex_t mutex;
pthread_cond_t cond_timer;
pthread_cond_t cond_sortzailea;
pthread_cond_t cond_scheduler;

int time_tick = 0;
int prozesua_sortzeko_baldintza = 3;   
int schedulerra_aktibatzeko_baldintza = 5;  
int stop = 0;
int cpu_exekutatzen = 0;

Queue prozesu_ilara; 

int main(){
    
    pthread_t tid_clock, tid_timer, tid_sortzailea, tid_scheduler; 
    
    // exekuzio-denbora segundutan
    const int execution_time = 20;

    // Hasieratu ahauzazko zenbakiak sortzeko pid-entzat
    srand(time(NULL)); 

    // Ilara hasieratu
    hasieratu_Queue(&prozesu_ilara);
    
    // clock-ean erabiliko diren gauzak hasieratu
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond_sortzailea, NULL);
    pthread_cond_init(&cond_scheduler, NULL);
    pthread_cond_init(&cond_timer, NULL); 
    
    // Hariak sortu
    printf("Clock-a abiarazten, Prozesu Sortzailea %ds-ro y Schedulerra %ds-ro. Exekutatzen %d segunduz...\n", prozesua_sortzeko_baldintza, schedulerra_aktibatzeko_baldintza, execution_time);
    pthread_create(&tid_clock, NULL, clock_fun, NULL);
    pthread_create(&tid_timer, NULL, timer_fun, NULL);
    pthread_create(&tid_sortzailea, NULL, process_creator_fun, NULL);
    pthread_create(&tid_scheduler, NULL, scheduler_aktibatzea, NULL);

    // Ejecución principal (el sistema funciona en segundo plano)
    sleep(execution_time); 
    
    // mutexa blokeatu eta aktibatu bukatzeko flag-a
    pthread_mutex_lock(&mutex); 
    stop = 1;
    
    // Hari guztiak esnatu ikusteko stop-a = 1 dela eta bukatu egin behar dutela.
    pthread_cond_broadcast(&cond_timer); 
    pthread_cond_broadcast(&cond_sortzailea);
    pthread_cond_broadcast(&cond_scheduler);
    pthread_mutex_unlock(&mutex);

    // Hari guztiak bukatzea itxaron
    pthread_join(tid_clock, NULL);
    pthread_join(tid_timer, NULL);
    pthread_join(tid_sortzailea, NULL);
    pthread_join(tid_scheduler, NULL);
    printf("\n[MAIN] Hariak geldituta\n");

    // Ilara garbitu exekutatu gabe gelditu diren prozesuetaz
    pthread_mutex_lock(&mutex); 
    ilara_garbitu(&prozesu_ilara);
    pthread_mutex_unlock(&mutex);
    
    // mutex-a eta kondizioak suntzitu
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond_sortzailea);
    pthread_cond_destroy(&cond_scheduler);
    pthread_cond_destroy(&cond_timer); 
    printf("[MAIN] Mutex eta kondizioak suntzituta. Programa bukatuta.\n");

    return 0;
} 
