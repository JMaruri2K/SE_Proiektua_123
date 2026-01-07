#ifndef PCB_ILARA_H
#define PCB_ILARA_H

#include <stdlib.h> 

typedef struct {

    uint32_t pgb;      // Page Table Base
    uint32_t code;     // .text hasierako helbide birtuala
    uint32_t data;     // .data hasierako helbide birtuala

} MemoryManagement;


typedef struct {
    int pid;
    int exekuzio_denbora;
    int billete_kopurua;

    MemoryManagement mm;

    
} PCB;

typedef struct NodoIlara {
    PCB* prozesua;
    struct NodoIlara *hurrengoa; 
} Nodoa;

typedef struct QueueManager {
    Nodoa *hasiera; 
    Nodoa *bukaera;
    int count;      
} Queue;

//PCB* sortu_prozesua();
void hasieratu_Queue(Queue *q);
void sartu_ilaran(Queue *q, PCB *prozesu_berria);
PCB* atera_ilaratik(Queue *q);
PCB* dado_txapelketa(Queue *q);
PCB* zozketa(Queue *q);
PCB* SJF(Queue *q);
void ilara_garbitu(Queue *q);


#endif // PCB_ILARA_H
