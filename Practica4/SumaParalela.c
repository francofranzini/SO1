#include <stdio.h>
#include <omp.h>
#include <stdlib.h>
#include "timing.h"
#define CANT_ELEMS 50000000
double suma                 = 0;
double suma_serial          = 0;
double arr[CANT_ELEMS];
float t;

void inicializar_array(){
  double x = 0.0;
  for(int i = 0; i<CANT_ELEMS; i++){
    arr[i] = x;
    x = x + 0.2;
  }
}


void fsuma(){
  
  #pragma omp parallel for
  for(int i = 0; i < CANT_ELEMS; i++){
    #pragma omp critical
    suma = suma + arr[i];
  } 
}
void fsuma_serial(){
  for(int i = 0; i<CANT_ELEMS; i++){
    suma_serial = suma_serial + arr[i];
  }
}

int main(){
  inicializar_array();
  TIME_void(fsuma(), &t);
  printf("El tiempo paralelo fue: %.10f\n", t);      
  TIME_void(fsuma_serial(), &t);
  printf("El tiempo serial fue: %.10f\n", t);        
  return 0;
}
