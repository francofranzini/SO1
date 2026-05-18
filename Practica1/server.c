#include "calculadora.h"
#include <stdio.h>

int* sumar_1_svc(operandos* argp, struct svc_req *rqstp){
  static int resultado;
  printf("Servidor: sumar %d + %d\n", argp->a, argp->b);
  resultado = argp->a + argp->b;
  return &resultado;
}