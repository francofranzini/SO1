// gcc shared_memory_producer_example.c -pthread -lrt -o shared_memory_producer_example

//C program for Producer process illustrating POSIX shared-memory API.
 
#include <stdio.h>
#include <fcntl.h> /* For O_* constants */
#include <sys/stat.h> /* For mode constants */
#include <sys/mman.h> /* mmap */
#include <unistd.h>  /* fork */
#include <sys/wait.h> /* wait */
#include <unistd.h> /* ftruncate */
 
int main()
{
    /* the size (in bytes) of shared memory object */
    const int PAGESIZE = 4096;
 
    /* name of the shared memory object */
    const char* name = "OS";
 
    /* shared memory file descriptor */
    int shm_fd;
 
    /* pointer to shared memory object */
    int* shared_memory; // esto podría ser void* sin problemas

    /* not shared memory */
    int not_shared_memory = 6;
 
    /* create the shared memory object */
    shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);
 
    /* configure the size of the shared memory object */
    ftruncate(shm_fd, PAGESIZE);
 
    /* memory map the shared memory object */
    shared_memory = mmap(0, PAGESIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);

    *shared_memory = 3;
 
    if (fork() == 0) {
        *shared_memory = 15;
        not_shared_memory = 8;
    }
    else {
        wait(NULL);
    }

    printf("Not Shared Memory: %d\n", not_shared_memory);
    printf("Shared Memory: %d\n", *shared_memory);

    
	shm_unlink(name); /* remueve el filedescriptor, la memoria sigue existiendo. Similar a la función close() de un archivo */
    return 0; //La región se desasigna automáticamente cuando finaliza el proceso.
}