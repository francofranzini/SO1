/*
Ej. 4. ¿Puede fallar la siguiente asercion (wr y rd corren en un thread cada uno)? Explique. Si puede
fallar, arregle el programa.
*/
volatile int x = 0;
volatile int y = 0;
void * wr(void *arg) { x = 123; y = 1; }
void * rd(void *arg) { while (!y); assert(x == 123);}