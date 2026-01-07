// memoria_fisikoa.c
#include "memoria_fisikoa.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

MemoriaFisikoa memoria_fisikoa;

void init_memoria_fisikoa() {
    memoria_fisikoa.memoria = (uint8_t*)calloc(MEMORY_SIZE, sizeof(uint8_t));
    
    if (memoria_fisikoa.memoria == NULL) {
        fprintf(stderr, "Ezin izan da memoria fisikoa esleitu (%d MB)\n", 
                MEMORY_SIZE / (1024 * 1024));
        exit(1);
    }
    
    pthread_mutex_init(&memoria_fisikoa.memory_mutex, NULL);
    
    printf("[MEMORIA] Memoria fisikoa hasieratuta: %d MB\n", 
           MEMORY_SIZE / (1024 * 1024));
}

void idatzi_hitza(uint32_t helbide_fisikoa, uint32_t value) {
    // Egiaztatu helbidea baliozkoa dela
    if (helbide_fisikoa + WORD_SIZE > MEMORY_SIZE) {
        fprintf(stderr, "Helbidea barrutitik kanpo: 0x%08X\n", helbide_fisikoa);
        return;
    }
    
    // Egiaztatu lerrokatuta dagoela (4ren multiploa)
    if (helbide_fisikoa % WORD_SIZE != 0) {
        fprintf(stderr, "Helbidea ez dago lerrokatuta: 0x%08X\n", helbide_fisikoa);
    }
    
    pthread_mutex_lock(&memoria_fisikoa.memory_mutex);
    
    // 4 byte-ak idatzi (big-endian formatuan)
    memoria_fisikoa.memoria[helbide_fisikoa]     = (value >> 24) & 0xFF;  // Byte esanguratsuena
    memoria_fisikoa.memoria[helbide_fisikoa + 1] = (value >> 16) & 0xFF;
    memoria_fisikoa.memoria[helbide_fisikoa + 2] = (value >> 8)  & 0xFF;
    memoria_fisikoa.memoria[helbide_fisikoa + 3] = value & 0xFF;          // Byte gutxien esanguratsua
    
    pthread_mutex_unlock(&memoria_fisikoa.memory_mutex);
}

uint32_t irakurri_hitza(uint32_t helbide_fisikoa) {
    // Egiaztatu helbidea baliozkoa dela
    if (helbide_fisikoa + WORD_SIZE > MEMORY_SIZE) {
        fprintf(stderr, "Helbidea barrutitik kanpo: 0x%08X\n", helbide_fisikoa);
        return 0;
    }
    
    pthread_mutex_lock(&memoria_fisikoa.memory_mutex);
    
    // 4 byte-ak irakurri eta konbinatu (big-end formatuan)
    uint32_t value = ((uint32_t)memoria_fisikoa.memoria[helbide_fisikoa]     << 24) |
                     ((uint32_t)memoria_fisikoa.memoria[helbide_fisikoa + 1] << 16) |
                     ((uint32_t)memoria_fisikoa.memoria[helbide_fisikoa + 2] << 8)  |
                     ((uint32_t)memoria_fisikoa.memoria[helbide_fisikoa + 3]);
    
    pthread_mutex_unlock(&memoria_fisikoa.memory_mutex);
    
    return value;
}

void free_memoria_fisikoa() {
    pthread_mutex_destroy(&memoria_fisikoa.memory_mutex);
    free(memoria_fisikoa.memoria);
    printf("[MEMORIA] Memoria fisikoa hutsituta\n");
}

// Memoria erakutsi (debug-erako)
void erakutsi_memoria(uint32_t hasiera, uint32_t amaiera) {
    printf("\n[MEMORIA] Edukia 0x%08X-tik 0x%08X-ra:\n", hasiera, amaiera);
    
    pthread_mutex_lock(&memoria_fisikoa.memory_mutex);
    
    for (uint32_t helb = hasiera; helb < amaiera && helb < MEMORY_SIZE; helb += 4) {
        uint32_t balioa = ((uint32_t)memoria_fisikoa.memoria[helb]     << 24) | ((uint32_t)memoria_fisikoa.memoria[helb + 1] << 16) | ((uint32_t)memoria_fisikoa.memoria[helb + 2] << 8)  | ((uint32_t)memoria_fisikoa.memoria[helb + 3]);
        printf("  0x%08X: 0x%08X (%u)\n", helb, balioa, balioa);
    }
    
    pthread_mutex_unlock(&memoria_fisikoa.memory_mutex);
}