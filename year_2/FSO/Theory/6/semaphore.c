#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

#define NTHREADS 4
#define ITERATIONS 40000000

int sharedVar = 0;
sem_t sem;

void *increment(void *iterp) {
    int iter = *(int *)iterp;

    for (int i = 0; i < iter; i++) {
        sem_wait(&sem);
        sharedVar++;
        sem_post(&sem);
    }
}

int main() {
    pthread_t th[NTHREADS];
    pthread_attr_t attr;
    pthread_attr_init(&attr);

    int sem_ret = sem_init(&sem, 0, 1);

    int iter = ITERATIONS / NTHREADS;

    for (int i = 0; i < NTHREADS; i++) {
        pthread_create(&th[i], &attr, increment, &iter);
    }

    for (int i = 0; i < NTHREADS; i++) {
        pthread_join(th[i], NULL);
    }

    printf("Global variable: %d\n", sharedVar);
    printf("It should be:    %d\n", ITERATIONS);
}