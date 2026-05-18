#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

sem_t tabaco, papel, fosforos, otra_vez;
int hay_tabaco 											= 0;
int hay_papel 											= 0;
int hay_fosforos                    = 0; 
pthread_mutex_t lock  							= PTHREAD_MUTEX_INITIALIZER;

/*
	a) Veamos un deadlock con el caso = 0:
		el segundo y tercer if, levantan el papel y tabaco, por lo que el fumador 1 puede avanzar si se le cede el control inmediatamente y sin interrupciones
		pero imaginemos que se transfiere el control al fumador 3, el cual consume papel, y luego al fumador 1, el cual consume el tabaco. Como ninguno fuma, 
		ninguno vuelve a llamar a el agente, y por ende el agente no vuelve a liberar recursos, y todos los fumadores quedan esperando recursos que estan en posesion
		de nadie ==> deadlock

	b) Como ya vimos en unidades anteriores, una de las formas que tenemos de evitar el deadlock es romper las dependencias circulares de recursos, es decir, estable
	cer un orden en el cual se consumiran los recursos para evitar que dos procesos/hilos esten sosteniendo recursos que necesitan otros recursos para soltarse.
	En este caso, al no haber una relacion de orden entre los productos necesarios para armar un cigarrillo, el orden es arbitrario y por ende elegimos
	tabaco -> papel -> fosforos
	De esta manera, tenemos que, por ejemplo, si liberamos tabaco y papel, queremos que se arme el fumador 1.
	Veamos uno a uno que pasa con estos recursos.
	En el caso del tabaco, ningun otro proceso puede tener como primer pedido de recursos al tabaco, ya que habria race-condition en el tabaco y por ende
	F1: tabaco -> papel
	F2: fosforos -> tabaco
	F3: papel -> fosforos
	El caso de como ordenamos el F3 en el ejemplo anterior, nos deja ver que pongamos como pongamos el sem_wait, tenemos race-condition con los fosforos o el papel
	ya que si 2 fumadores intentan agarrar los fosforos -> race condition
	y si el papel es el primer recurso pedido por F3, se produce race-condition entre F1 luego del tabaco y F3

	Como el orden dado es generico, vemos que esta situacion se repite para cada uno de los ordenes posibles ==> no es posible ordenarlos para evitar la race-condition
*/

void agente()
{
	while (1) {
		sem_wait(&otra_vez);
		int caso = random() % 3;
		if (caso != 0) sem_post(&fosforos);
		if (caso != 1) sem_post(&papel);
		if (caso != 2) sem_post(&tabaco);
	}
}

void fumar(int fumador)
{
	printf("Fumador %d: Puf! Puf! Puf!\n", fumador);
	sleep(1);
}
void * fumador1(void *arg)  //Tiene FOSFOROS
{
	fumar(1);
	sem_post(&otra_vez);
	return NULL;
}

void * fumador2(void *arg) //Tiene PAPEL
{
	fumar(2);
	sem_post(&otra_vez);
	return NULL;


}

void * fumador3(void *arg) //Tiene TABACO
{
	fumar(3);
	sem_post(&otra_vez);
	return NULL;

}

void* pusher_fosforos(void *arg){
	while(1){
		sem_wait(&fosforos);
		pthread_mutex_lock(&lock);
		if(hay_papel){
		  hay_papel = 0;
			fumador3((void*)0);
		}
		else if(hay_tabaco){
			hay_tabaco = 0;
			fumador2((void*)0);
		}
		else hay_fosforos = 1;
		pthread_mutex_unlock(&lock);
	}
}
void* pusher_papel(void *arg){
	while(1){
		sem_wait(&papel);
		pthread_mutex_lock(&lock);
		if(hay_tabaco){
			hay_tabaco = 0;
			fumador1((void*)0);
		}
		else if(hay_fosforos){
			hay_fosforos = 0;
			fumador3((void*)0);
		}
		else hay_papel = 1;
		pthread_mutex_unlock(&lock);

	}
}
void* pusher_tabaco(void *arg){
	while(1){
		sem_wait(&tabaco);
		pthread_mutex_lock(&lock);
		if(hay_papel){
			hay_papel = 0;
			fumador1((void*) 0);
		} else if(hay_fosforos){
			hay_fosforos = 0;
			fumador2((void*)0);
		} else hay_tabaco = 1;
		pthread_mutex_unlock(&lock);

	}
}




int main()
{
	pthread_t s1, s2, s3, p1, p2, p3;

	pthread_mutex_init(&lock, NULL);
	sem_init(&tabaco,   0, 0);
	sem_init(&papel,    0, 0);
	sem_init(&fosforos, 0, 0);
	sem_init(&otra_vez, 0, 1);

	pthread_create(&p1, NULL, pusher_fosforos, NULL);
	pthread_create(&p2, NULL, pusher_papel, NULL);
	pthread_create(&p3, NULL, pusher_tabaco, NULL);

	// pthread_create(&s1, NULL, fumador1, NULL);
	// pthread_create(&s2, NULL, fumador2, NULL);
	// pthread_create(&s3, NULL, fumador3, NULL);

	agente();

	return 0;
}
