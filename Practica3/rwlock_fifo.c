#include "rwlock_fifo.h"
#include <stdlib.h>

void rwlock_init(rwlock_t *rw) {
    rw->lectores_activos = 0;
    rw->escribiendo      = 0;
    rw->cola_head        = NULL;
    rw->cola_tail        = NULL;
    pthread_mutex_init(&rw->lock, NULL);
}

void rwlock_destroy(rwlock_t *rw) {
    pthread_mutex_destroy(&rw->lock);
}

// encolar un waiter y dormir hasta que le toque
static void encolar_y_esperar(rwlock_t *rw, int es_escritor) {
    waiter_t w;
    w.es_escritor  = es_escritor;
    w.puede_entrar = 0;
    w.next         = NULL;
    pthread_cond_init(&w.cond, NULL);

    // agregar al final de la cola
    if (rw->cola_tail)
        rw->cola_tail->next = &w;
    else
        rw->cola_head = &w;
    rw->cola_tail = &w;

    // dormir hasta que alguien nos despierte
    while (!w.puede_entrar)
        pthread_cond_wait(&w.cond, &rw->lock);

    pthread_cond_destroy(&w.cond);
}

// despertar al siguiente en la cola según las reglas
static void despertar_siguiente(rwlock_t *rw) {
    if (!rw->cola_head) return;  // cola vacía

    if (rw->cola_head->es_escritor) {
        // el primero es escritor: despertarlo solo a él
        waiter_t *w    = rw->cola_head;
        rw->cola_head  = w->next;
        if (!rw->cola_head) rw->cola_tail = NULL;
        w->puede_entrar = 1;
        pthread_cond_signal(&w->cond);

    } else {
        // el primero es lector: despertar a todos los lectores
        // contiguos al frente de la cola
        while (rw->cola_head && !rw->cola_head->es_escritor) {
            waiter_t *w   = rw->cola_head;
            rw->cola_head = w->next;
            if (!rw->cola_head) rw->cola_tail = NULL;
            w->puede_entrar = 1;
            pthread_cond_signal(&w->cond);
        }
    }
}

void read_lock(rwlock_t *rw) {
    pthread_mutex_lock(&rw->lock);

    // puede entrar si: no hay escritor activo Y la cola está vacía
    // (cola vacía garantiza FIFO: no salteamos a nadie esperando)
    if (!rw->escribiendo && !rw->cola_head) {
        rw->lectores_activos++;
    } else {
        encolar_y_esperar(rw, 0);
        rw->lectores_activos++;
    }

    pthread_mutex_unlock(&rw->lock);
}

void read_unlock(rwlock_t *rw) {
    pthread_mutex_lock(&rw->lock);

    rw->lectores_activos--;

    // si era el último lector, despertar al siguiente en la cola
    if (rw->lectores_activos == 0)
        despertar_siguiente(rw);//Con el lock, signalea a todos los lectores en cola hasta el primer escritor
        //Les dice ya podes despertar, y cuando suelte el lock empezaran a leer

    pthread_mutex_unlock(&rw->lock);
}

void write_lock(rwlock_t *rw) {
    pthread_mutex_lock(&rw->lock);

    // puede entrar si: no hay nadie leyendo, no hay escritor, y no hay cola
    if (!rw->escribiendo && !rw->lectores_activos && !rw->cola_head) {
        rw->escribiendo = 1;
    } else {
        encolar_y_esperar(rw, 1); //Espera el lock
        //Obtuvo el lock
        rw->escribiendo = 1;
    }

    pthread_mutex_unlock(&rw->lock);
}

void write_unlock(rwlock_t *rw) {
    pthread_mutex_lock(&rw->lock);

    rw->escribiendo = 0;
    despertar_siguiente(rw); //Signalea, pero no suelta el lock

    pthread_mutex_unlock(&rw->lock);
}