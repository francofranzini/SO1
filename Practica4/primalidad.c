#include <stdio.h>
#include <omp.h>
#include <stdlib.h>
#include "timing.h"
#define CANT_ELEMS 50000000
double suma                 = 0;
double suma_serial          = 0;
double arr[CANT_ELEMS];
float t;

int raiz(int n){
  int i;
  for(i = 1; i< n/i; i++);
  return i;
}

int es_primo_s(int x){
  for(int i = 2; i<= raiz(x); i++){
    if(x%i == 0) return 0;
  }
  return 1;
}

int es_primo_paralelo(int x){
  int y = 0;
  #pragma omp parallel for reduction(||: y)
    for(int i = 2; i <= raiz(x); i++){y = y || (x%i == 0);}
  return y;
}


int main(){
  int primo = 100000007;
  TIME_void(es_primo_s(primo), &t);
  printf("El tiempo serial fue: %.10f\n", t);        
  TIME_void(es_primo_paralelo(primo), &t);
  printf("El tiempo paralelo fue: %.10f\n", t);      
  return 0;
}
