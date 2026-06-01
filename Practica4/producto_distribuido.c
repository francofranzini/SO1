#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include "timing.h"

#define N 5000// definir
float t;
double t1, t2, t3, t4;
int i, j;
int A[N][N], v[N], x[N];
void calculo_paralelo(int id, int procesos, int* local_x, int* local_A, int lineas_por_proc){
	//Divido el array A de a N elementos

	MPI_Scatter(&A, N*lineas_por_proc, MPI_INT, local_A, N*lineas_por_proc, MPI_INT, 0, MPI_COMM_WORLD);
	for(i=0; i<lineas_por_proc; i++){
		local_x[i] = 0;
		for(j=0; j<N; j++)
			local_x[i] += local_A[N*i + j] * v[j];
	}
	//Cada proceso calcula su porcion en local_x a partir de lo que se obtuvo en el scatter de A, en local_A
	//Now we need to gather all the calculations into x
	MPI_Gather(
		local_x,			//Were we did the calculations
		lineas_por_proc,		//how many numbers
		MPI_INT,			//number type
		x,					//where are we gathering
		lineas_por_proc,		//how many numbers
		MPI_INT,			//numbers type
		0,
		MPI_COMM_WORLD
	);
}
int calculo_secuencial()
{
	/*Leer A y v*/
	for (i=0;i<N;i++) {
		x[i]=0;
		for (j=0;j<N;j++)
			x[i] += A[i][j]*v[j];
	}
	return 0;
}

int main(int argc, char **argv)
{
	for(int i = 0; i<N; i++) {for(int j = 0; j<N; j++) A[i][j] = rand()%2; v[i] = 1;}
	TIME_void(calculo_secuencial(), &t); printf("El tiempo en secuencial fue: %.10f\n", t);


	// MPI_Init(&argc, &argv);
    // int rank, size;
    // MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	// MPI_Comm_size(MPI_COMM_WORLD, &size);
	// t1 = MPI_Wtime();
	// int lineas_por_proc = N / size; 
	// int local_x[lineas_por_proc];
	// int *local_A = malloc(lineas_por_proc * N * sizeof(int));

    // calculo_paralelo(rank, size, local_x, local_A, lineas_por_proc);
	// free(local_A);
    // if(rank == 0){
	// 	t2 = MPI_Wtime();
    //     printf("El tiempo en paralelo fue: %.10f\n", t2 - t1);
    // }

    // MPI_Finalize();
	
	return 0;
}
/*
	Registro de tiempos:
	tiempo secuencial (ts) = 0.0347899310
	1 core: 0.0763758070 --> Su = ts/(tp1) = 0.45
	2 core: 0.0571015380 --> Su = ts/(tp2) = 0.60
	3 core: 0.0498018470 --> Su = ts/(tp3) = 0.70


*/