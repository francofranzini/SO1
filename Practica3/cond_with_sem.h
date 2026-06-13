#ifndef COND_WITH_SEM
#define COND_WITH_SEM
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>


typedef struct _cond{
    sem_t lock;
    sem_t condition;
    int waiters;
}cond;

/*
    It creates a condition
*/
cond* cond_create();
/*
    Waits for the signal
*/
void cond_wait(cond* c);
/*
    Sends only one signal
*/
void cond_signal(cond* c);
/*
    Sends signal to all processes waiting 
*/
void cond_broadcast(cond* c);
/*
    Destroys the condition
*/
void cond_destroy(cond* c);

#endif