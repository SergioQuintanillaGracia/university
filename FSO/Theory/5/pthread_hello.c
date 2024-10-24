#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

void *myPrint(void *msg) {
    printf((char *) msg);
}

int main() {
    pthread_t th1, th2;
    pthread_attr_t attr;

    pthread_attr_init(&attr);
    pthread_create(&th1, &attr, myPrint, "Hello\n");
    sleep(1);
}