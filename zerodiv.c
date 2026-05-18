#include<stdlib.h>
#include <stdio.h>
#include <signal.h>


int denom = 0;
void handler(int s){printf("outc!\n"); denom = 1;}

int main(){


  int r;
  // -----------------
  signal(SIGFPE, handler); 
  //Cuando se produzca la señal "SIGPE", lo va a manejar handler
  r = 5/denom;
  //Cuando r = 1/denom, no salta SIGFPE, por algun motivo
  printf("r: %d\n", r);

  return 0;
}