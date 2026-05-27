#include <stdio.h>
#include <omp.h>
#include <stdlib.h>
#include "timing.h"
#define CANT_VISITANTES 10000000

int visitantes = 0;




int main(){
  
  #pragma omp parallel for
  for(int i = 0; i<CANT_VISITANTES; i++){
    #pragma omp critical
    visitantes = visitantes + 1;
  }
  

  printf("Visitantes: %i\n", visitantes);

  return 0;
}