/*
  a) El programa puede terminar en deadlock en los casos donde
  los threads todos intentan tomar los tenedores al mismo tiempo
  pues el F0 toma el lock de F1 y asi sucesivamente
  F0 -> F1 -> F2 -> F3 -> ... -> FN -> F0 por ende todos tienen el 
  tenedor derecho, pero luego, cualquiera que intente tomar el izquierdo
  quedara esperando indefinidamente, y asi lo haran todos entrando en un deadlock
  donde su vecino suelta el tenedor cuando el otro lo suelte y asi sucesivamente
  b) Si invertimos el orden de toma de cubiertos de un filosofo, el caso donde todos toman
  el tenedor derecho se ve interrumpido por el que tomo el de la izquierda, puesto que es el 
  de la derecha de alguien. Asi, este filosofo podra tomar ambos cubiertos comer, y en el caso
  extremo, sera el de su derecha el primero en empezar a comer, y asi sucesivamente.
  Solo con invertir el orden de toma de algun filosofo se cumple, pues suponiendo que todos intentan
  tomar el tenedor a la vez, al llegar el momento del zurdo, tomara el tenedor de su izquierda, siguiendo
  disponible el de su derecha, o su izquierdo estara ocupado, y esperara a este para tomar el de la derecha
  dejando disponible su tenedor derecho para el comensal a su derecha, el cual podra comer
*/
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#define N_FILOSOFOS 5
#define ESPERA 500

sem_t sem;

pthread_mutex_t tenedor[N_FILOSOFOS];
pthread_mutex_t * izq(int i) { return &tenedor[i]; }
pthread_mutex_t * der(int i) { return &tenedor[(i+1) % N_FILOSOFOS]; }
void pensar(int i)
{
  printf("Filosofo %d pensando...\n", i);
  usleep(random() % ESPERA);
}
void comer(int i)
{
  printf("Filosofo %d comiendo...\n", i);
  usleep(random() % ESPERA);
}
void tomar_tenedores(int i)
{
  pthread_mutex_lock(der(i));
  pthread_mutex_lock(izq(i));
}
void dejar_tenedores(int i)
{
  pthread_mutex_unlock(der(i));
  pthread_mutex_unlock(izq(i));
}
void * filosofo(void *arg)
{
  int i = arg - (void*)0;
  while (1) {
    sem_wait(&sem);
    tomar_tenedores(i);
    comer(i);
    dejar_tenedores(i);
    sem_post(&sem);
    pensar(i);
  }
}
int main()
{
  sem_init(&sem, 0, N_FILOSOFOS-1);

  pthread_t filo[N_FILOSOFOS];
  int i;
  for (i = 0; i < N_FILOSOFOS; i++) pthread_mutex_init(&tenedor[i], NULL);
  for (i = 0; i < N_FILOSOFOS; i++) pthread_create(&filo[i], NULL, filosofo, i + (void*)0);
  pthread_join(filo[0], NULL);
  
  sem_destroy(&sem);
  return 0;
}
/*
  d) Esto podria producir un Livelock, ya que en el caso donde todos tomen el de la derecha, 
  luego no encuentren posible tomar el de la izquierda, soltaran el de la derecha y esto puede repetirse indefinidamente,
  hasta que el orden no determinista los haga comer donde uno encuentre ambos cubiertos disponibles.
  Esto puede disminuirse usando espera aleatoria, pero existe riesgo de starvation, es busy wait
*/