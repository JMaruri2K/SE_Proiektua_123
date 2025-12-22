#ifndef CLOCK_H
#define CLOCK_H

#include <pthread.h>
#include "pcb_ilara.h" 


extern pthread_mutex_t mutex;
extern pthread_cond_t cond_creator;      
extern pthread_cond_t cond_scheduler;    
extern pthread_cond_t cond_tick_reset;    

extern int time_tick;               // denbora kontatzeko
extern int prozesua_sortzeko_baldintza;
extern int schedulerra_aktibatzeko_baldintza;
extern int cpu_exekutatzen;

extern int stop; // gelditzeko baldintza


extern Queue prozesu_ilara; 

// Prototipo de la función del hilo Clock (implementada en clock.c)
void *clock_fun(void *arg);

#endif // CLOCK_H
