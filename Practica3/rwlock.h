#ifndef RWLOCK_H
#define RWLOCK_H

#include <pthread.h>

typedef struct {
    int             lectores_activos;
    int             escribiendo;
    pthread_mutex_t lock;
    pthread_cond_t  ok_leer;
    pthread_cond_t  ok_escribir;
} rwlock_t;

void rwlock_init   (rwlock_t *rw);
void rwlock_destroy(rwlock_t *rw);
void read_lock     (rwlock_t *rw);
void read_unlock   (rwlock_t *rw);
void write_lock    (rwlock_t *rw);
void write_unlock  (rwlock_t *rw);

#endif