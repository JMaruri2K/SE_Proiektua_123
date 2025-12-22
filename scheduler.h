#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <pthread.h>
#include "clock.h" // Beste programen gauzak importatzeko


void *scheduler_aktibatzea(void *arg);
void *exekutatu_prozesua(PCB *arg);

#endif // SCHEDULER_H 
