/*
Ej. 1. Suponga que queremos detectar si un array A contiene el entero 42 iterando por el mismo, y si es
asi, prender la bandera encontrado. ¿Hay diferencia entre los dos fragmentos siguientes? ¿En que casos?

-1- 

if (A[i] == 42) encontrado = true;

-2-
encontrado = encontrado || (A[i] == 42);r

La version 2 es peligrosa en concurrencia ya que puede llegar a escribir encontrado en falso cuando otro
thread lo puso en verdadero
t1 -> encontrado = false 
t2 -> encontrado = false
t1 -> escribe encontrado = true
t2 -> (ya leyo encontrado) escribe encontrado en false
*/

