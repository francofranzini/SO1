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
  //mi valor es my_id
  //MPI_Send(&d, 1, MPI_INT, my_id+1, 0 ,MPI_COMM_WORLD);
  int suma = 0;
  int aux;

  //pensarlo como anillo ... 5 -> 0 -> 1 -> 2 -> ... -> 5 -> 0 -> 

  for(int i = 0; i < num_procs; i++){
    if(my_id == i){
      MPI_Send(&my_id, 1, MPI_INT, (my_id+1)%num_procs,0, MPI_COMM_WORLD);
      MPI_Recv(&aux, 1, MPI_INT, (num_procs + my_id -1)%num_procs, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      suma+= aux;
    }
    else{
      MPI_Recv(&aux, 1, MPI_INT, (num_procs + my_id -1)%num_procs, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      suma+= aux;
      MPI_Send(&aux, 1, MPI_INT, (my_id+1)%num_procs,0, MPI_COMM_WORLD);
    }
    
  }
  printf("Soy el proceso: %i y la suma es: %i\n", my_id, suma);




  
  MPI_Finalize();
}

//Un comunicador es una interfaz que permite la comunicacion entre procesos
// donde cada una no interfiere con la otra

