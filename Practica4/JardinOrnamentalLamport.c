#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#define CANT_VISITANTES 100000000

// pthread_mutex_t lock          = PTHREAD_MUTEX_INITIALIZER;

int visitantes                = 0;
int turno[2]                  = {0};
int eligiendo[2]              = {0};

int max(int x, int y) {if(x>y) return x; return y;}


void* molinete(void* k){
  int i = *(int*)k;
  while(1){
    //Lock
    eligiendo[i] = 1;
    turno[i] = 1+ max(turno[0], turno[1]);
    eligiendo[i] = 0;
    //Tengo asignado un turno que puede ser el mismo que el de otro proceso
    for(int j = 0; j < 2; j++){
      //1) Espero que el proceso con el que me comparo termine de elegir
      while(eligiendo[j]){;}//Busywait
      //2) El proceso con el que me comparo tiene numero y dejo pasar o no tiene numero
      while(((turno[j] != 0) && (turno[j] < turno[i])) || ((turno[j] == turno[i]) && j < i)) {;} //Busy wait
    }
    //Verifique que soy el menor de todos y deje pasar a los de menor PID
    /*
      Seccion critica
    */
    if(visitantes == CANT_VISITANTES){
      turno[i] = 0;
      return (void*)0;
    }
    visitantes++;
    //Unlock
    turno[i] = 0;
  }
}


int main(){

  pthread_t m1, m2;
  int i = 0, j = 1;
  pthread_create(&m1, NULL, molinete, &i);
  pthread_create(&m2, NULL, molinete, &j);

  pthread_join(m1, NULL);
  pthread_join(m2, NULL);

  printf("Visitantes: %i\n",visitantes );
  return 0;
}