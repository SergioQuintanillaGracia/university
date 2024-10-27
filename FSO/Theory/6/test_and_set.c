#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define NTHREADS 4
#define ITERATIONS 40000000

int sharedVar = 0;
int lock = 0;

void *increment(void *iterp) {
    int iter = *(int *)iterp;

    for (int i = 0; i < iter; i++) {
        while (__sync_lock_test_and_set(&lock, 1));
        sharedVar++;
        __sync_lock_release(&lock);
    }
}

int main() {
    pthread_t th[NTHREADS];
    pthread_attr_t attr;
    pthread_attr_init(&attr);

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