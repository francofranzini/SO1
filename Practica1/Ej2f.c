#define POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include <sys/mman.h>

#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>

int main(void)
{
  int a[100];
  a[1] = 0;
  a[99] = 1;
  pid_t pid = fork();
  
  if(pid == 0) {
    printf("SOY EL HIJO y mi PID: %d\n", getpid());
    return 0;
  }
  if(pid != 0){
    pause();
    printf("SOY el padre y mi PID: %d\n", getpid());
  }
  return 0;
}