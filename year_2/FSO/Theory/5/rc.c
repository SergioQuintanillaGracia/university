#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define MAX 4

int globalVariable;

void *addition(void *ptr) {
    int auxVariable;
    long *iter = (long *) ptr;

    for (int i = 0; i < *iter; i++) {
        globalVariable++;
    }
}

int main(int argc, char *argv[]) {
    long iterations = 20000;
    int nth;
    pthread_t thr[MAX];
    pthread_attr_t attr;

    if (argc == 1) nth = 1;
    else nth = atoi(argv[1]);

    if (nth > MAX) nth = MAX;

    globalVariable = 0;
    printf("Number of threads: %d\n", nth);
    pthread_attr_init(&attr);

    for (int i = 0; i < nth; i++) {
        pthread_create(&thr[i], &attr, addition, &iterations);

        // Without the sleep, the result is inconsistent due to a race condition.
        // Multiple threads are accessing and modifying globalVariable concurrently.
        // Each thread may read the current value of globalVariable into a register,
        // increment it, and store it back to memory. However, if the OS preempts a thread
        // after it has read globalVariable but before it writes the updated value back to memory,
        // another thread might modify globalVariable in the meantime.
        // When the preempted thread resumes, it overwrites the newer value with the stale value
        // it had read earlier, leading to incorrect results.
        // Sleep helps prevent this by giving each thread a chance to finish its operation
        // before another thread runs, but it does not solve the underlying race condition.
        sleep(1);
    }

    for (int i = 0; i < nth; i++) {
        pthread_join(thr[i], NULL);
    }

    printf("globalVariable: %d\n", globalVariable);
}