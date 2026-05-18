#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int main(void)
{
  pid_t pid1 = fork();
  pid_t pid2 = fork();

  printf("pid1: %d pid2: %d \n", pid1, pid2);
  // // int a = 1;
  // if(pid == -1) printf("ERROR\n");
  // if(pid == 0){
  //   printf("Soy el hijo\n");
  //   execl("./hola", NULL);
  // }
  // else{
  //   printf("Soy el padre, el hijo tiene pid %d\n", pid);
  //   wait(0);
  // }
  return 0;
}
