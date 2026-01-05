#ifndef CLOCK_H
#define CLOCK_H

#include <pthread.h>
#include "pcb_ilara.h" 

// Extern deklaratu beste programa guztiek ez hasieratzeko haien aldagaiak iadanik main-ean daude eginda.
extern pthread_mutex_t mutex;
extern pthread_cond_t cond_timer;  
extern pthread_cond_t cond_sortzailea;      
extern pthread_cond_t cond_scheduler;    
extern pthread_cond_t cond_tick_reset;    

extern int time_tick;               // denbora kontatzeko
extern int prozesua_sortzeko_baldintza;
extern int schedulerra_aktibatzeko_baldintza;
extern int cpu_exekutatzen;

extern int stop; // gelditzeko baldintza


extern Queue prozesu_ilara; 

// deklaratu beste programe erabili dezaten
void *clock_fun(void *arg);

#endif // CLOCK_H
