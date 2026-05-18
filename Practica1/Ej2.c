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
  execl("./hola", "./hola", NULL, NULL, NULL);
  printf("%d\n", getpid());

  return 0;
}
