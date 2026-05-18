#include <unistd.h> /* Write/Read/lseeki/fork */
#include <stdio.h> /* prints */
#include <sys/wait.h> /* wait */
#include <sys/types.h> /* pid_t */



// Interpretación del valor de status de wait(&status)
// Con exit(1), stat de wait(&stat) es 256.
// Esto se debe a que el valor de retorno de wait codifica tanto el estado de salida del proceso como el motivo por el que terminó el proceso.
// En general, no se debe intentar interpretar directamente los valores de retorno distintos de cero de una llamada wait().
// En cambio, se debee usar las macros WIF.... Por ejemplo, para chequear si un proceso terminó normalmente:


int main(int argc, char *argv[])
{
  int stat;

  char* prog = argv[0];
   
  int pid = fork();
  if (pid == 0) //child
  {
    pid_t pid_child = getpid();
    printf("child %d running...\n", pid_child);

    for(int i=0; i < 100; ++i)
    {
      printf("iteration: %d\n", i);
    }
  }
  else //parent
  {
    pid_t pid_parent = getpid();
    printf("parent %d waits \n", pid_parent);
    // take one argument status and returns 
    // a process ID of dead children.
    wait(&stat);

    if (WIFEXITED(stat)) // WIFEXITED returns true if the child terminated normally
      printf("Exit status: %d\n", WEXITSTATUS(stat)); // WEXITSTATUS returns the exit status of the child.
    else if (WIFSIGNALED(stat)) // WIFSIGNALED returns true if the child process was terminated by a signal.
      psignal(WTERMSIG(stat), "Exit signal"); // WTERMSIG returns the number of the signal that caused the child process to terminate.
  }
  return 0;
}