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
  
  if(my_id == 0){
    printf("Enter a number to broadcast: "); scanf("%d", &d);
  }
  else{
    printf("Process %d: Before BCAST, value d is %d\n", my_id, d);
  }
  MPI_Bcast(&d, 1, MPI_INT, 0, MPI_COMM_WORLD);

  printf("process %d: after BCAST d is %d\n", my_id, d);

  MPI_Finalize();
}

//Un comunicador es una interfaz que permite la comunicacion entre procesos
// donde cada una no interfiere con la otra

