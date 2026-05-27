#define _XOPEN_SOURCE 600
#include <stdio.h>
#include <mpi.h>


/*
  SCATTER divide una estructura en porciones y distribuye eso en muchos procesos
*/
int datos[12] = {0};
int scatteredData[2];
void main(int argc, char** argv){
  int num_procs, my_id, status;
  int d;
  //No se comparte memoria, se copia todo el segmento de datos
  //en cada proceso
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &my_id);
  MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
  if(my_id == 0){

    for(int i = 0; i<12; i++){
      datos[i] = i;
    }
  }

  MPI_Scatter(&datos, 2, MPI_INT, &scatteredData, 2, MPI_INT, 0, MPI_COMM_WORLD);

  for(int i = 0; i<2; i++){
    printf("P%d, recibi %d\n",my_id, scatteredData[i]);
  }

  MPI_Finalize();
}

//Un comunicador es una interfaz que permite la comunicacion entre procesos
// donde cada una no interfiere con la otra

