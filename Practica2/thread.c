#include <pthread.h>
#include <stdio.h>

void* hola_mundo(void* arg){
  printf("HOLA MUNDO\n");
}

int main(){

  pthread_t prueba;
  pthread_create(&prueba, NULL, hola_mundo, NULL);

  pthread_join(prueba, NULL);

  printf("NO SOY EL THREAD\n");

  return 0;
}