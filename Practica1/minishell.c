#define POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>

#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>

int main(void)
{
  char* comando = NULL;
  size_t len = 0;
  
  while(1){
    printf("> ");
    fflush(stdout);

    ssize_t nread = getline(&comando, &len, stdin);
    if(nread == -1){
      break;
    }
    if(comando[nread-1] == '\n'){
      comando[nread-1] = '\0';
    } 
    
    //Parseamos comandos
    char* comandos[20];
    int j = 0;
    comandos[j] = strtok(comando, "|");
    while(comandos[j] != NULL){
      j++;
      comandos[j] = strtok(NULL, "|");
    }
    //comandos[] = [c1, c2, c3,...]

    int prev_fd = STDIN_FILENO;
    /*
      Para cada comando del pipe, lo desgloso en comando + args, e ingreso su salida en el pipe
    */
    for(int k = 0; k<j-1; k++){
      char* args[100];
      int i = 0;
      int write_to = -1;
      args[i] = strtok(comandos[k], " ");
      while(args[i] != NULL){
        if(*args[i] == '>') write_to = i;
        i++;
        args[i] = strtok(NULL, " ");
      } 
  
      /*
        Creo un proceso hijo para la ejecucion del comando.

        [read-end ... PIPE ... write-end]
      */
      
      int pipefd[2];
      pipe(pipefd);
      int pid = fork();
  
      if(pid == 0){
        //HIJO
        
        /*
          Tomo como stdin a prev_fd
        */
        dup2(prev_fd, STDIN_FILENO);
        /*
          Si no es el ultimo comando, conectamos el stdout con la write-end del pipe
        */
        if (comandos[k+1] != NULL) {
          dup2(pipefd[1], STDOUT_FILENO);
        }


  
        //Vamos a redirigir la salida stdout a nuestro nuevo archivo
        if(write_to >= 1){
          int fd = open(args[write_to + 1], O_CREAT | O_WRONLY | O_TRUNC, 0644);
          //Lo que haremos, sera redirigir la salida estandar al archivo, mediante el comando dup2
          dup2(fd, 1);
          args[write_to] = NULL; 
        }
        
  
        close(pipefd[0]);close(pipefd[1]);
        //comando lleva el nombre del comando que sera buscado en $PATH, y args de los argumentos que
        //acompañan la ejecucion de exec, en nuestro caso, son flags.
        execvp(args[0], args);
        perror("execvp");
        exit(0);
      }
      else{
        /*
          Leo el stdin desde la salida del pipe
        */
        close(pipefd[1]);
        prev_fd = pipefd[0];
      }
    }
    /*
      Ejecucion del ultimo comando
    */
    char* args[100];
    int i = 0;
    int write_to = -1;
    args[i] = strtok(comandos[j-1], " ");
    while(args[i] != NULL){
      if(*args[i] == '>') write_to = i;
      i++;
      args[i] = strtok(NULL, " ");
    }
    int pid = fork();
    
    if(pid == 0){
      //HIJO
      dup2(prev_fd, STDIN_FILENO);
      //Vamos a redirigir la salida stdout a nuestro nuevo archivo
      if(write_to >= 1){
        int fd = open(args[write_to + 1], O_CREAT | O_WRONLY | O_TRUNC, 0644);
        //Lo que haremos, sera redirigir la salida estandar al archivo, mediante el comando dup2
        dup2(fd, 1);
        args[write_to] = NULL; 
      }

      //comando lleva el nombre del comando que sera buscado en $PATH, y args de los argumentos que
      //acompañan la ejecucion de exec, en nuestro caso, son flags.
      execvp(args[0], args);
      perror("execvp");
      exit(0);
    }
    else{
      wait(0);
    }
  }
  return 0;
}
