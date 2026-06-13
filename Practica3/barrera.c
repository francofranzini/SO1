#include "barrera.h"


void barrier_init(struct barrier* b, int n){
    b->n      = n;
    pthread_mutex_init(&b->lock, NULL);
    pthread_cond_init(&b->wakeup, NULL);
}
void barrier_wait(struct barrier* b){
    pthread_mutex_lock(&b->lock);
    b->n--;
    if(b->n <= 0) pthread_cond_broadcast(&b->wakeup);
    else          pthread_cond_wait(&b->wakeup, &b->lock);
    
    pthread_mutex_unlock(&b->lock);
}