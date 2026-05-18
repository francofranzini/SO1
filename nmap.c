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
  int sh = shm_open("anashefile", O_CREAT, 0755);
  printf("%d\n", sh);

  return 0;
}
