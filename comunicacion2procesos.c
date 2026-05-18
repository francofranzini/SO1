#include<stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>


void handler(int s){printf("Soy el hijo\n");}
int main(){

  pid_t pid = fork();
  
  signal(SIGCHLD, handler);
  if(pid != 0){
    wait(0);
    printf("Soy el padre\n");
  }


  return 0;
}