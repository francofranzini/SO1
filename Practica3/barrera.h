#ifndef BARRIER_H
#define BARRIER_H
#include <pthread.h>

struct barrier{
    int             n;
    pthread_mutex_t lock;
    pthread_cond_t  wakeup;
};

/*
    Creates a barrier 
*/
void barrier_init(struct barrier *b, int n);
/*
    The process stop until n processes are waiting
*/
void barrier_wait(struct barrier *b);

#endif