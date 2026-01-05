#include "timer.h"
#include <stdio.h>

void *timer_fun(void *arg) {
    while (stop == 0) {

        pthread_mutex_lock(&mutex);

        // Clock-aren seinalea itxaron
        pthread_cond_wait(&cond_timer, &mutex);

        if (stop == 1) {
            pthread_mutex_unlock(&mutex);
            break;
        }

        // Prozesu_Sortzailea: 3 segunduro
        if (time_tick % prozesua_sortzeko_baldintza == 0) {
            pthread_cond_signal(&cond_sortzailea);
        }

        // Schedulerra: 5 segunduro CPU librea badago
        if (time_tick % schedulerra_aktibatzeko_baldintza == 0 && cpu_exekutatzen == 0) {
            pthread_cond_signal(&cond_scheduler);
        }

        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}