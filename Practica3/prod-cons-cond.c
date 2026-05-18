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
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t hay_lugar = PTHREAD_COND_INITIALIZER, hay_elems = PTHREAD_COND_INITIALIZER;
int tope = 0;

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
		sleep(random() % 3);
		pthread_mutex_lock(&lock);
		while(tope>= SZ) pthread_cond_wait(&hay_lugar, &lock);
		//----------Zona critica-----------
		int *p = malloc(sizeof *p);
		*p = random() % 100;
		printf("Productor %d: produje %p->%d\n", id, p, *p);
		enviar(p);
		//-------------------------------
		pthread_cond_signal(&hay_elems);
		pthread_mutex_unlock(&lock);
	}
	return NULL;
}

void * cons_f(void *arg)
{
	int id = arg - (void*)0;
	while (1) {
		sleep(random() % 3);
		pthread_mutex_lock(&lock);
		while(tope == 0) pthread_cond_wait(&hay_elems, &lock);
		/*---------------ZONA CRITICA--------------------*/
		int *p = recibir();
		printf("Consumidor %d: obtuve %p->%d\n", id, p, *p);
		free(p);
		//-------------------------------------
		pthread_cond_signal(&hay_lugar);
		pthread_mutex_unlock(&lock);
		
	}
	return NULL;
}

int main()
{
	pthread_mutex_init(&lock, NULL);
	pthread_cond_init(&hay_lugar, NULL);
	pthread_cond_init(&hay_elems, NULL);

	pthread_t productores[M], consumidores[N];
	int i;

	for (i = 0; i < M; i++)
		pthread_create(&productores[i], NULL, prod_f, i + (void*)0);

	for (i = 0; i < N; i++)
		pthread_create(&consumidores[i], NULL, cons_f, i + (void*)0);

	pthread_join(productores[0], NULL); /* Espera para siempre */
	return 0;
}
