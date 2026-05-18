#include <pthread.h>
typedef struct waiter {
    int             turno;
    int             es_escritor;
    int             puede_entrar;   // señal individual para este waiter
    pthread_cond_t  cond;           // condición individual
    struct waiter  *next;
} waiter_t;

typedef struct {
    int             lectores_activos;
    int             escribiendo;
    pthread_mutex_t lock;
    waiter_t       *cola_head;      // frente de la cola FIFO
    waiter_t       *cola_tail;      // fin de la cola FIFO
} rwlock_t;