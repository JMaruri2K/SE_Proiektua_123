// memoria_fisikoa.h
#ifndef MEMORIA_FISIKOA_H
#define MEMORIA_FISIKOA_H

#include <stdint.h>
#include <pthread.h>

#define MEMORY_SIZE (1 << 24)        // 2^24 bytes = 16 MB
#define WORD_SIZE 4                   // 4 bytes hitz bakoitzeko
#define PAGE_SIZE 4096                // 4 KB orri bakoitzeko
#define NUM_PAGES (MEMORY_SIZE / PAGE_SIZE)
#define KERNEL_RESERVED_PAGES 256    // Kernelari reaserbatutako orriak

typedef struct {
    uint8_t *memoria;                 // Byten array-a
    pthread_mutex_t memory_mutex;
} MemoriaFisikoa;

extern MemoriaFisikoa memoria_fisikoa;

void init_memoria_fisikoa();
void idatzi_hitza(uint32_t helbide_fisikoa, uint32_t value);
uint32_t irakurri_hitza(uint32_t helbide_fisikoa);
void free_memoria_fisikoa();
void erakutsi_memoria(uint32_t hasiera, uint32_t amaiera);

#endif