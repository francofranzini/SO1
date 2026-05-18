#define _XOPEN_SOURCE 600
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#define SILLAS 5


typedef struct _Cliente{
  int                           cliente_nro;
  int                           llamado; 
  pthread_cond_t                cond;
  pthread_barrier_t             barrera_corte;  // ← sincroniza cortar/me_cortan
  pthread_barrier_t             barrera_pago;   // ← sincroniza pagar/me_pagan
  struct _Cliente               *sig;
}Cliente;


void me_cortan(int n)   { printf("Cliente %d: me están cortando\n", n); sleep(2); }
void cortar(int n)      { printf("Barbero: cortando al cliente %d\n", n); sleep(2); }
void pago(int n)        { printf("Cliente %d: pagando\n", n); sleep(1); }
void recibir_pago(int n){ printf("Barbero: recibiendo pago del cliente %d\n", n); sleep(1); }

int ocupadas                   = 0;
pthread_mutex_t lock           = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t despertar       = PTHREAD_COND_INITIALIZER;
Cliente* primero               = NULL,
       * ultimo                = NULL;

Cliente* cliente_init(int n){
  Cliente* cliente = malloc(sizeof(Cliente));
  cliente->cliente_nro = n;
  cliente->llamado = 0;
  pthread_cond_init(&cliente->cond, NULL);
  pthread_barrier_init(&cliente->barrera_corte, NULL, 2);  // ← 2: cliente + barbero
  pthread_barrier_init(&cliente->barrera_pago,  NULL, 2);
  return cliente;
}

int llega_cliente(Cliente* cliente){
  pthread_mutex_lock(&lock);
  if(ocupadas < SILLAS){
    ocupadas++;
    if(!primero){
      primero       = cliente;
      ultimo        = cliente;
    }
    else{
      ultimo->sig   = cliente;
      ultimo        = cliente;
    }
    pthread_cond_signal(&despertar);
    pthread_mutex_unlock(&lock);
    return 0;
  }
  printf("Soy el cliente %d, me fui\n", cliente->cliente_nro);
  pthread_mutex_unlock(&lock);

  return 1; // NO ENCONTRO SILLA
}

void* cliente(void* arg){
  int i;
  int n = arg - (void*)0;
  Cliente* cliente = cliente_init(n);
  if(llega_cliente(cliente)){
    free(cliente);
    return (void*)0;
  } 
  pthread_mutex_lock(&lock);
  while(!cliente->llamado) 
    pthread_cond_wait(&cliente->cond, &lock);
  pthread_mutex_unlock(&lock);
  //ME CORTAN
  me_cortan(cliente->cliente_nro);
  pthread_barrier_wait(&cliente->barrera_corte);
  //PAGO
  pago(cliente->cliente_nro);
  pthread_barrier_wait(&cliente->barrera_pago);
  //LIMPIEZA
  pthread_barrier_destroy(&cliente->barrera_corte);
  pthread_barrier_destroy(&cliente->barrera_pago);
  pthread_cond_destroy(&cliente->cond);
  free(cliente);
}
void* barbero(){
  int i;
  while(1){
    //HACER PASAR AL CLIENTE
    pthread_mutex_lock(&lock);
    while(ocupadas == 0){
      pthread_cond_wait(&despertar, &lock);
    }
    
    Cliente* cliente_a_cortar = primero;
    primero = primero->sig;
    if(!primero) ultimo = NULL;
    ocupadas--;
    cliente_a_cortar->llamado = 1;
    pthread_cond_signal(&cliente_a_cortar->cond);

    pthread_mutex_unlock(&lock);
    //CORTAR EL PELO AL CLIENTE
    cortar(cliente_a_cortar->cliente_nro);
    pthread_barrier_wait(&cliente_a_cortar->barrera_corte);
    //RECIBIR DINERO
    recibir_pago(cliente_a_cortar->cliente_nro);
    pthread_barrier_wait(&cliente_a_cortar->barrera_pago);
  }
}



int main() {
    pthread_t hilo_barbero;
    pthread_t hilos_clientes[100];

    pthread_create(&hilo_barbero, NULL, barbero, NULL);

    for (int i = 0; i < 100; i++) {
        sleep(rand() % 5);   // clientes llegan en momentos distintos
        pthread_create(&hilos_clientes[i], NULL, cliente, (void*)(long)i);
    }

    for (int i = 0; i < 100; i++)
        pthread_join(hilos_clientes[i], NULL);
    return 0;
}
