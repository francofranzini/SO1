#include "cond_with_sem.h"


cond* cond_create(){
    cond* c = malloc(sizeof(cond));
    sem_init(&c->lock, 0, 1);
    sem_init(&c->condition, 0, 0);
    c->waiters = 0;
    return c; 
}

void cond_wait(cond* c){
    sem_wait(&c->lock);
    c->waiters++;
    sem_post(&c->lock);

    sem_wait(&c->condition);

    sem_wait(&c->lock);
    c->waiters--;
    sem_post(&c->lock);
}

void cond_signal(cond* c){
    sem_wait(&c->lock);
    if(c->waiters > 0){
        sem_post(&c->condition);
    }
    sem_post(&c->lock);
}

void cond_broadcast(cond* c){
    sem_wait(&c->lock);
    for(int i = 0; i<c->waiters; i++) sem_post(&c->condition);
    sem_post(&c->lock);
}

void cond_destroy(cond* c){
    sem_destroy(&c->lock);
    sem_destroy(&c->condition);
}

