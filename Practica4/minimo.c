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
  arr[0] = 5.0;
  arr[CANT_ELEMS-1] = 0.0;
}

void min_serial(){
  int x = arr[0];
  for(int i = 0; i<CANT_ELEMS; i++){
    x = x < arr[i] ? x : arr[i];
  }
}
void min_paralelo(){
  int x = arr[0];
  #pragma omp parallel for reduction(min: x)
  for(int i = 0; i<CANT_ELEMS; i++){
    if(arr[i] < x) x = arr[i];
  } 
}


int main(){
  inicializar_array();
  TIME_void(min_serial(), &t);
  printf("El tiempo paralelo fue: %.10f\n", t);      
  TIME_void(min_paralelo(), &t);
  printf("El tiempo serial fue: %.10f\n", t);        
  return 0;
}
