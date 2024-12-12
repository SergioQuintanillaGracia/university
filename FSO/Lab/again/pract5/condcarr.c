/*                                                                      */
/*  CondCarr.c                                                          */
/*  ObservaciÃ³n de condiciones de carrera y soluciones                  */
/*  Fundamentos de los Sistemas Operativos  (sesiÃ³n 5)                  */
/*  Grupo de Sistemas Operativos DISCA (UPV)                            */
/*  Ãšltima revisiÃ³n: 21/10/2024                                         */
/*                                                                      */


#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <semaphore.h>
#include <unistd.h>
#include <math.h>


/*
 REPETICIONES : Numero de veces que se suma/resta 1 a V
*/
#define REPETICIONES      20000000

/*
   VARIABLES GLOBALES (COMPARTIDAS) 
*/

long int V = 100;      // Valor inicial

/* 
   FUNCIONES AUXILIARES
   test_and_set(int * objetivo) : devuelve 1 (cierto) si llave esta siendo utilizada, 
                                  devuelve 0 (falso) si llave esta libre.
*/
int test_and_set(int *spinlock) {
  int ret;
  __asm__ __volatile__(
    "xchg %0, %1"
    : "=r"(ret), "=m"(*spinlock)
    : "0"(1), "m"(*spinlock)
    : "memory");
  return ret;
}

void SeccionRestante(int V)
{
    int i;
    long tot;
  
    tot = 0;
    for (i = 0; i < 300; i++) {
       tot = tot+V;
    }
}


/* 
   FUNCIONES QUE EJECUTAN LAS TAREAS o THREADS
   - Agrega : Ejecuta un bucle donde incrementa la variable V 
   - Resta  : Ejecuta un bucle donde decrementa la variable V
   - Inspecciona : Imprime cada segundo el valor de V
*/

void *agrega (void *argumento) {

  long int cont;
       
  for (cont = 0; cont < REPETICIONES; cont = cont + 1) {
  
      V = V + 1;
    
      SeccionRestante(V);

  }
 
  printf("-------> Fin AGREGA (V = %ld)\n", V);
  pthread_exit(0);
}

void *resta (void *argumento) {

  long int cont;
  
  for (cont = 0; cont < REPETICIONES; cont = cont + 1) {
        
        V = V - 1;
    
        SeccionRestante(V);
    
  }

  printf("-------> Fin RESTA  (V = %ld)\n", V);
  pthread_exit(0);
}

void *inspecciona (void *argumento) {
 
  for (;;) {
    usleep(200000);
    fprintf(stderr, "Inspeccion: Valor actual de V = %ld\n", V);
  } 
}

//*PROGRAMA PRINCIPAL  

int main (int argc, char *argv[]) {
  //Declaracion de  variables 
    pthread_t hiloSuma, hiloResta, hiloInspeccion;
    pthread_attr_t attr;   

  // Inicilizacion de los atributos de los hilos (por defecto)
    pthread_attr_init(&attr);
    
  // EJERCICIO: Cree los tres hilos propuestos con dichos atributos 

  // EJERCICIO: Hilo principal debe espera a que las
  // tareas "agrega" y "resta" finalicen 

    
  // Fin del programa principal
  fprintf(stderr, "-------> VALOR FINAL: V = %ld\n\n", V);
  exit(0);
}
