#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define M 5
#define N 5
#define SZ 8

/*
 * El buffer guarda punteros a enteros, los
 * productores los consiguen con malloc() y los
 * consumidores los liberan con free()
 */
int *buffer[SZ];
int tope = 0;
sem_t vacios, llenos, lock;

void enviar(int *p)
{
	buffer[tope++] = p;
	return;
}

int * recibir()
{
	return buffer[--tope];
}

void * prod_f(void *arg)
{
	int id = arg - (void*)0;
	while (1) {
		sleep(random() % 2);
		sem_wait(&vacios);
		sem_wait(&lock);
		//----------Zona critica-----------
		int *p = malloc(sizeof *p);
		*p = random() % 100;
		printf("Productor %d: produje %p->%d\n", id, p, *p);
		enviar(p);
		//-------------------------------
		sem_post(&lock);
		sem_post(&llenos);
	}
	return NULL;
}

void * cons_f(void *arg)
{
	int id = arg - (void*)0;
	while (1) {
		sleep(random() % 2);
		/*
			Sincronizacion del buffer
		*/
		sem_wait(&llenos); 
		sem_wait(&lock);
		/*---------------ZONA CRITICA--------------------*/
		int *p = recibir();
		printf("Consumidor %d: obtuve %p->%d\n", id, p, *p);
		free(p);
		//-------------------------------------
		sem_post(&lock);
		sem_post(&vacios);
		
	}
	return NULL;
}

int main()
{
	sem_init(&llenos, 0, 0); // Arranca vacio
	sem_init(&vacios, 0, SZ);
	sem_init(&lock, 0, 1);
	pthread_t productores[M], consumidores[N];
	int i;

	for (i = 0; i < M; i++)
		pthread_create(&productores[i], NULL, prod_f, i + (void*)0);

	for (i = 0; i < N; i++)
		pthread_create(&consumidores[i], NULL, cons_f, i + (void*)0);

	pthread_join(productores[0], NULL); /* Espera para siempre */
	return 0;
}
