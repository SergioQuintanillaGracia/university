#include <stdio.h>
#include <pthread.h>

int globalVar = 0;

void *addTimes2000000(void *n) {
    for (int i = 0; i < 2000000; i++) {
        globalVar += *(int *)n;
    }
}

int main() {
    pthread_t th1, th2;
    pthread_attr_t attr;
    int n = 1;

    pthread_create(&th1, &attr, addTimes2000000, &n);
    pthread_create(&th2, &attr, addTimes2000000, &n);
    pthread_join(th1, NULL);
    pthread_join(th2, NULL);
    
    printf("Global variable: %d\n", globalVar);
}