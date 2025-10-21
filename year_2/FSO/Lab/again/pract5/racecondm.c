 
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <semaphore.h>
#include <unistd.h>
#include <math.h>


#define REPETITIONS 20000000   // CONSTANT

// GLOBAL SHARED VARIABLES 
long int V = 100;      // Initial value
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

// AUXILIARY FUNTIONS
int test_and_set(int *spinlock) {
    int ret;
    __asm__ __volatile__(
    "xchg %0, %1"
    : "=r"(ret), "=m"(*spinlock)
    : "0"(1), "m"(*spinlock)
    : "memory");
    return ret;
}

void RemainingSeccion(int V)
{
    int i;
    long tot;
  
    tot = 0;
    for (i = 0; i < 300; i++) {
       tot = tot+V;
    }
}


// THREAD FUNCTIONS
void *inc (void *parameter) {
    long int cont;
    for (cont = 0; cont < REPETITIONS; cont = cont + 1) {  
        pthread_mutex_lock(&mutex);
        V = V + 1;
        pthread_mutex_unlock(&mutex);

        RemainingSeccion(V);
    }
    printf("-------> inc end (V = %ld)\n", V);
    pthread_exit(0);
}

void *dec (void *parameter) {
    long int cont;
    for (cont = 0; cont < REPETITIONS; cont = cont + 1) {
        pthread_mutex_lock(&mutex);
        V = V - 1;
        pthread_mutex_unlock(&mutex);
    
        RemainingSeccion(V);           
    }
    
    printf("-------> dec end (V = %ld)\n", V);
    pthread_exit(0);
}

void *inspec (void *parameter) {
    for (;;) {
        usleep(200000);
        fprintf(stderr, "Inspec: actual value of V = %ld\n", V);
    } 
}

// MAIN FUNCTION  
int main (void) {
    // Declaring the requiered variables
    pthread_t incThread, decThread, inspecThread;
    pthread_attr_t attr;   
  
    // Default thread attributes
    pthread_attr_init(&attr);
 
    // EXERCISE: Create threads inc, dec and inspec with attr attributes 
    pthread_create(&incThread, &attr, inc, NULL);
    pthread_create(&decThread, &attr, dec, NULL);
    pthread_create(&inspecThread, &attr, inspec, NULL);
    // EXERCISE: The main thread has to wait inc and dec threads to end
    pthread_join(incThread, NULL);
    pthread_join(decThread, NULL);

    // Main program end
    fprintf(stderr, "-------> FINAL VALUE: V = %ld\n\n", V);
    exit(0);
}   
