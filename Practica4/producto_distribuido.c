#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#define N 6// definir
int local_A[N][N];
int main(int argc, char **argv)
{
	int i, j;
	int A[N][N], v[N], x[N];
	for(int i = 0; i<N; i++) {for(int j = 0; j<N; j++) A[i][j] = rand()%2; v[i] = 1;}
	// printf("A is: \n");
	// for(int i = 0; i<N; i++) {for(int j = 0; j<N; j++) printf("%i ",A[i][j]); printf("\n");}
	
	/*Leer A y v*/
	int procesos, id;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &id);
	MPI_Comm_size(MPI_COMM_WORLD, &procesos);
	//Divido el array A de a N elementos
	int lineas_por_proc = N / procesos; 
	MPI_Scatter(&A, N*lineas_por_proc, MPI_INT, &local_A, N*lineas_por_proc, MPI_INT, 0, MPI_COMM_WORLD);

	int local_x[lineas_por_proc];
	for(i=0; i<lineas_por_proc; i++){
		local_x[i] = 0;

		for(j=0; j<N; j++)
			local_x[i] += local_A[i][j] * v[j];
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
	MPI_Finalize();

	for(int i = 0; i<N; i++) printf("%i ", x[i]);
	printf("\n");

	/*Escribir x */
	return 0;
}