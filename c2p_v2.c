#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <stdio.h>


void handler(int s){printf("Soy el hijo\n");}
int main(){

  struct sigaction act;
  act.sa_handler = handler;
  sigaction(SIGUSR1, &act, NULL);
  pid_t pid = fork();
  if(pid != 0){
    kill(pid, SIGUSR1);
    wait(NULL);
    printf("Soy el padre\n");
  }

  return 0;
}