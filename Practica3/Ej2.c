/*
a1 --> b2
b1 --> a2
*/
Thread A
{
  a1();
  tomar(lock);
  A1 = True;
  cond_signal(c1);
  soltar(lock);

  tomar(lock); //Toma el lock
  while(!B1) cond_wait(c2); //Si tiene el lock, verifica B1, sino vuelve a esperar
  soltar(lock); //suelta el lock, ya ocurrio b1
  a2;
}
Thread B
{
  b1(); //Ocurre en cualquier momento
  tomar(lock) ;//luego de b1, tomo el lock para avisarle a A que se ejecuto 
  B1 = True;
  cond_signal(c2); //Lo despierto en caso de haber esperado
  soltar(lock) // Lo dejo ejecutarse
  
  tomar(lock)
  while(!A1) cond_wait(c1);
  soltar(lock)
  b2();

}