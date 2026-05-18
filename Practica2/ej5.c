#include <pthread.h>
#include <stdio.h>
#include <pthread.h>
#define N_VISITANTES 5000000
/*
  Tenemos que garantizar que los procesos accedan de a 1 a la seccion critica
  Si agregamos flags por ejemplo, la flag tambien pasa a la seccion critica


  Veamos la idea de turnos:
    - 0 -> molinete 1
    - 1 -> molinete 2
  El molinete 1 le pasa el turno al molinete 2 y viceversa.
  Problemas: secuencializado, alternados, y cuando un thread termina
  el otro no puede seguir funcionando


  Ausencia de Deadlock (Liveness)
  Si hay procesos intentando tomar/soltar un lock, algun proceso va a tomar/soltar
  el lock.
  
  Una solucion posible es que flag sea una solicitud de acceso a la 
  seccion critica. flag[thread] = {0, 1}

  Lo que hacemos es verificar si el otro proceso tiene intencion de entrar, si lo tiene
  cedemos el turno. Esto no es ausente de deadlock

  Lo que podemos hacer es una combinacion entre intencion de acceso a la seccion critica
  y turno

  Ausencia de Inanicion: Siempre que un proceso quiere tomar un lock, eventualmente lo hara

  Demostracion de Peterson (Algoritmo de acceso a la Region Critica) y Ausencia de Deadlock

  
*/
int visitantes = 0;
int turno = 0;
int intents[2] = {0, 0};

void* molinete1(void* arg){
  for(int i = 0; i<N_VISITANTES/2; ++i){
    intents[0] = 1;
    turno = 1;
    asm("mfence");
    while(intents[1] && turno == 1){;} //Busy waiting
    //seccion critica
    visitantes = visitantes + 1;
    printf("Molinete 1: %i \n", visitantes);
    //unlock
    intents[0] = 0;
  }
}
void* molinete2(void* arg){
  for(int i = 0; i<N_VISITANTES/2; ++i){
    intents[1] = 1;
    turno = 0;
    asm("mfence");
    while(intents[0] && turno == 0){;} //Busy waiting
    //seccion critica
    visitantes = visitantes + 1;
    printf("Molinete 2: %i\n", visitantes);
    //unlock
    intents[1] = 0;
  }
}

int main(){

  pthread_t t1,t2;

  pthread_create(&t1, NULL, molinete1, NULL);
  pthread_create(&t2, NULL, molinete2, NULL);

  pthread_join(t1, NULL);
  pthread_join(t2, NULL);


  // pthread_join(prueba, NULL);

  // printf("NO SOY EL THREAD\n");

  return 0;
}