#define _XOPEN_SOURCE 600
#include <stdio.h>
#include <mpi.h>


/*
  Send y recv son bloqueantes, espera a que se reciba y esperan a recibir

  Barrier

  Broadcast: MPI_Bcast broadcast de datos 
*/

void main(int argc, char** argv){
  int num_procs, my_id, status;
  int d;
  //No se comparte memoria, se copia todo el segmento de datos
  //en cada proceso
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &my_id);
  MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
  if(my_id == 0){ //soy el root
    scanf("%d", &d);
    MPI_Send(&d, 1, MPI_INT, my_id+1, 0 ,MPI_COMM_WORLD);
    MPI_Recv(&d, 1, MPI_INT, (num_procs - 1), 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
  }
  else{
    MPI_Recv(&d, 1, MPI_INT, my_id-1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    d++;
    printf("El valor es: %d\n", d);
    MPI_Send(&d, 1, MPI_INT, (my_id+1)%num_procs, 0, MPI_COMM_WORLD);
  }
  //Guarda en &my_id el id del proceso
  //Guarda en &num_procs la cantidad de procesadores
  MPI_Finalize();
}

//Un comunicador es una interfaz que permite la comunicacion entre procesos
// donde cada una no interfiere con la otra

