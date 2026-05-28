#include <pthread.h>
#include <stdio.h>
#include "timing.h"
#include <math.h>
#include <stdlib.h>

#define SIZE 16000
int ua1[SIZE], ua2[SIZE], ua3[SIZE];
float t;

typedef struct{
  int* a;
  int n;
}args;

void swap(int* x1, int* x2){
  int x = *x1;
  *x1 = *x2;
  *x2 = x;
}


/* Particion de Lomuto, tomando el primer elemento como pivote */
int particionar(int a[], int n)
{
  int i, j = 0;
  int p = a[0];
  swap(&a[0], &a[n-1]);
  for (i = 0; i < n-1; i++) {
    if (a[i] <= p)
    swap(&a[i], &a[j++]);
  }
  swap(&a[j], &a[n-1]);
  return j;
}
void qsort_seq(int a[], int n)
{
  if (n < 2)  return;
  int p = particionar(a, n);

  qsort_seq(a, p);
  qsort_seq(a+p+1, n-p-1);
  
}
void qsort_omp(int a[], int n){
  if (n < 2)  return;
  int p = particionar(a, n);
  
  #pragma omp parallel sections
  {
    #pragma omp section
    qsort_omp(a, p);
    #pragma omp section
    qsort_omp(a+p+1, n-p-1);
  }
}
void* qsort_pth(void* ar)
{
  args* arg = (args*)ar;
  int n = arg->n;
  int* a = arg->a;
  if (n < 2)  return (void*)0;
  int p = particionar(a, n);
  pthread_t t1, t2;
  args *args1;
  args *args2;
  args1 = malloc(sizeof(args));
  args2 = malloc(sizeof(args));
  args1->a = a;     args1->n = p;
  args2->a = a+p+1; args2->n = n-p-1;

  pthread_create(&t1, NULL, (void*)qsort_pth, args1);
  pthread_create(&t2, NULL, (void*)qsort_pth, args2);
  pthread_join(t1, NULL);
  pthread_join(t2, NULL);
  free(args1);
  free(args2);


  return NULL;
}
int main(){
  for(int i = 0; i<SIZE; i++){
    ua1[i] = rand()%100;
    ua2[i] = ua1[i];
    ua3[i] = ua1[i];
  }
  args *args1;
  args1 = malloc(sizeof(args));
  args1->a = ua2;     args1->n = SIZE;
  TIME_void(qsort_seq(ua1, SIZE), &t);
  printf("El tiempo serial fue: %.10f\n", t);
  TIME_void(qsort_omp(ua3, SIZE), &t);
  printf("El tiempo omp fue: %.10f\n", t); 
  TIME_void(qsort_pth(args1), &t);
  printf("El tiempo con pthread fue: %.10f\n", t);
  

}