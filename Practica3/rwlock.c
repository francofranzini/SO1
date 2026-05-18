#include "rwlock.h"

typedef struct {
    int lectores_activos;      // cuántos lectores están leyendo ahora
    pthread_mutex_t lock;      // protege lectores_activos
    pthread_cond_t  ok_leer;   // para despertar lectores esperando
    pthread_cond_t  ok_escribir; // para despertar escritores esperando
    int escribiendo;           // hay un escritor activo?
    int quiere_escribir;
} rwlock_t;

void rwlock_init(rwlock_t* rwl){
  rwl->lectores_activos = 0;
  rwl->escribiendo = 0;
  rwl->quiere_escribir = 0;
  pthread_mutex_init(&rwl->lock, NULL);
  pthread_cond_init(&rwl->ok_escribir, NULL);
  pthread_cond_init(&rwl->ok_leer, NULL);
}
void rwlock_destroy(rwlock_t *rw) {
  pthread_mutex_destroy(&rw->lock);
  pthread_cond_destroy (&rw->ok_leer);
  pthread_cond_destroy (&rw->ok_escribir);
}




void read_lock(rwlock_t *rw) {
  pthread_mutex_lock(&rw->lock);

  // esperar solo si hay un escritor activo
  while (rw->escribiendo || rw->quiere_escribir)
      pthread_cond_wait(&rw->ok_leer, &rw->lock);

  // si no hay escritor, entrar inmediatamente (read-preferring)
  rw->lectores_activos++;

  pthread_mutex_unlock(&rw->lock);
}

void read_unlock(rwlock_t *rw) {
  pthread_mutex_lock(&rw->lock);

  rw->lectores_activos--;

  // si era el último lector, avisar a escritores esperando
  if (rw->lectores_activos == 0)
      pthread_cond_signal(&rw->ok_escribir);

  pthread_mutex_unlock(&rw->lock);
}

void write_lock(rwlock_t *rw) {
  pthread_mutex_lock(&rw->lock);

  // esperar si hay lectores O hay otro escritor
  if (rw->lectores_activos > 0 || rw->escribiendo){
    rw->quiere_escribir++;
    while(rw->lectores_activos > 0 || rw->escribiendo)
      pthread_cond_wait(&rw->ok_escribir, &rw->lock);
    rw->quiere_escribir--;
  }
  rw->escribiendo = 1;

  pthread_mutex_unlock(&rw->lock);
}

void write_unlock(rwlock_t *rw) {
  pthread_mutex_lock(&rw->lock);

  rw->escribiendo = 0;

  if(rw->quiere_escribir){
    pthread_cond_signal(&rw->ok_escribir); 
  }
  else{
    pthread_cond_broadcast(&rw->ok_leer);
  }

  pthread_mutex_unlock(&rw->lock);
}