#include <stdlib.h>
#include <stdio.h>
#include <omp.h>
#include "timing.h"
#define N 500000000
#define TASK_SIZE 1000
int x[N], y[N];
float t;
void merge(int a[], int lo, int mid, int hi)
{
  int n = hi - lo + 1;

  int* aux = malloc(sizeof(int) * n);

  int x = lo;
  int y = mid + 1;

  int i = 0;

  
  while(x <= mid && y <= hi){

    if(a[x] <= a[y])
      aux[i++] = a[x++];
    else
      aux[i++] = a[y++];
  }

  while(x <= mid)
    aux[i++] = a[x++];

  while(y <= hi)
    aux[i++] = a[y++];

  for(i = 0; i < n; i++)
    a[lo + i] = aux[i];

  free(aux);
}



void* ms_seq(int a[N], int x, int y){
  if(y-x <= 0) return NULL;
  int mid = (x + y)/2;
  
  ms_seq(a, x, mid);
  ms_seq(a, mid+1, y);
  merge(a, x, mid, y);

  return NULL;
}
void* ms_omp(int a[N], int x, int y){
  if(y-x <= 0) return NULL;
  int mid = (x + y)/2;
  
  int n = mid - x;
  #pragma omp task shared(a) if (n > TASK_SIZE)
  ms_omp(a, x, mid);

  #pragma omp task shared(a) if (n > TASK_SIZE)
  ms_omp(a, mid+1, y);


  #pragma omp taskwait
  merge(a, x, mid, y);
  return NULL;
}

int main(){

  for(int i = 0; i<N; i++){
    x[i] = rand()%1000;
    y[i] = x[i];
  }
  TIME_void(ms_seq(x, 0, N), &t);
  printf("El tiempo serial fue: %.10f\n", t);
  #pragma omp parallel
  {
    #pragma omp single
    TIME_void(ms_omp(y, 0, N-1), &t);
  }
  printf("El tiempo paralelo fue: %.10f\n", t);
  // for(int i = 0; i<N; i++){
  //   printf("%i ",y[i]);
  // }
  

  return 0; 
}
