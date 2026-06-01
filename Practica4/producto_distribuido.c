#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#define N 6// definir
int scattered[N];
int main(int argc, char **argv)
{
	int i, j;
	int A[N][N], v[N], x[N];
	for(int i = 0; i<N; i++) for(int j = 0; j<N; j++) A[i][j] = rand()%100;
	printf("A is: \n");
	for(int i = 0; i<N; i++) {for(int j = 0; j<N; j++) printf("%i ",A[i][j]); printf("\n");}
	
	/*Leer A y v*/
	int procesos, id;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &id);
	MPI_Comm_size(MPI_COMM_WORLD, &procesos);
	//Divido el array A de a N elementos
	MPI_Scatter(&A, N, MPI_INT, &scattered, N, MPI_INT, 0, MPI_COMM_WORLD);
	x[id] = 0;
	for(int i = 0; i<N; i++) x[id] += scattered[i]*v[i];

	// printf("Soy el proceso %i: y scattered es: %i\n", id, scattered[1]);
	MPI_Gather()?
	MPI_Finalize();

	/*Escribir x */
	return 0;
}