#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>
#include "timing.h"

#define N 1000
float t;
int A[N][N], B[N][N], C[N][N], D[N][N], E[N][N];
void mult(int A[N][N], int B[N][N], int C[N][N])
{
  int i, j, k;
  for (i = 0; i < N; i++)
  for (j = 0; j < N; j++)
  for (k = 0; k < N; k++)
  C[i][j] += A[i][k] * B[k][j];
}

void mult_mp(int A[N][N], int B[N][N], int C[N][N]){

  #pragma omp parallel for collapse(2)
  for(int i = 0; i < N; i++){

    for(int j = 0; j < N; j++){

      int suma = 0;

      for(int k = 0; k < N; k++){
        suma += A[i][k] * B[k][j];
      }

      C[i][j] = suma;
    }
  }
}

void mult_opt(int A[N][N], int B[N][N], int C[N][N]){

  for(int i = 0; i < N; i++){
    for(int k = 0; k < N; k++){
      int a = A[i][k];
      for(int j = 0; j < N; j++){
        C[i][j] += a * B[k][j];
      }
    }
  }
}

int main()
{
  int i, j;
  for (i = 0; i < N; i++) {
    for (j = 0; j < N; j++) {
      A[i][j] = rand() % 1000;
      B[i][j] = rand() % 1000;
      D[i][j] = 0;
      E[i][j] = 0;
    }
  }
  
  TIME_void(mult(A, B, C), &t);
  printf("El tiempo serial fue: %.10f\n", t);
  TIME_void(mult_opt(A, B, D), &t);
  printf("El tiempo serial optimizado fue: %.10f\n", t);      
  TIME_void(mult_mp(A, B, E), &t);
  printf("El tiempo paralelo fue: %.10f\n", t);
  return 0;
}
