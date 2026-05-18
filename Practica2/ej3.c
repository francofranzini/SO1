#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <assert.h>
/*
Ej. 3. ¿Puede fallar la siguiente asercion? ¿Bajo que condiciones? Explique. Si puede fallar, arregle el
programa.
*/

int x = 0, y = 0, a = 0, b = 0;
void * foo(void *arg) { x = 1; a = y; return NULL; }
void * bar(void *arg) { y = 1; b = x; return NULL; }
int main() {
  pthread_t t0, t1;
  pthread_create(&t0, NULL, foo, NULL);
  pthread_create(&t1, NULL, bar, NULL);
  pthread_join(t0, NULL);
  pthread_join(t1, NULL);
  assert (a || b);
  return 0;
}
/*
  Quizas lo que puede estar pasando es que t2 precargue en registros los valores de x = 0, y en medio de la ejecucion se 
  ejecute t1 hasta el final, cargando a = y = 0 y luego, a pesar de que t1 hizo x =1, t2 ya cargo en la pila el 0 
  y por ende a||b = False.
*/