#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

void *task1(void *delay) {
    for (int i = 0; i < 10; i++) {
        printf("Task 1: %d\n", i);
        usleep(*(int *)delay * 1000);
    }
    printf("Task 1 finished\n");
}

void *task2(void *delay) {
    for (int i = 0; i < 10; i++) {
        printf("    Task 2: %d\n", i);
        usleep(*(int *)delay * 1000);
    }
    printf("    Task 2 finished\n");
}

int main() {
    pthread_t th1, th2;
    pthread_attr_t attr;

    int delay1 = 500;
    int delay2 = 1000;

    if (pthread_attr_init(&attr) != 0) {
        printf("Attribute error\n");
        exit(1);
    }

    if (pthread_create(&th1, &attr, task1, &delay1) != 0) {
        printf("Error creating thread 1\n");
        exit(1);
    }

    if (pthread_create(&th2, &attr, task2, &delay2) != 0) {
        printf("Error creating thread 2\n");
        exit(1);
    }

    printf("Threads created, waiting for termination\n");
    pthread_join(th1, NULL);
    pthread_join(th2, NULL);
    
    printf("Finished");
}