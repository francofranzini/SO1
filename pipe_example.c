#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h> // exit()
#include <string.h> // strlen()

/*
The following program creates a pipe, and then fork(2)s to create a child process; the child inherits a duplicate set of file descriptors that refer to the same pipe. 
After the fork(2), each process closes the file descriptors that it doesn't need for the  pipe  (see pipe(7)).
The  parent  then  writes the string to the pipe, and the child reads this string a byte at a time from the pipe and echoes it on standard output
*/

// gcc pipe_example.c -o pipe_example

int main(void)
{
        int     fd[2], nbytes;
        pid_t   childpid;
        char    string[] = "Hello, world!\n";
        char    readbuffer[80];

        pipe(fd);
        
        if((childpid = fork()) == -1)
        {
                perror("fork");
                exit(1);
        }

        if(childpid == 0)
        {
                /* Child process closes up read-end side of pipe */
                close(fd[0]);

                /* Send "string" through the output side of pipe */
                write(fd[1], string, (strlen(string)+1));
                exit(0);
        }
        else
        {
                /* Parent process closes up write-end side of pipe */
                close(fd[1]);

                /* Read in a string from the pipe */
                nbytes = read(fd[0], readbuffer, sizeof(readbuffer));
                printf("Received string: %s", readbuffer);
        }
        
        return 0;
}
