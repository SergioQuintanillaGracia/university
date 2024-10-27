#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

pthread_t th1, th2;
pthread_attr_t atrib;

void *Func_th1(void *arg)
{
    int i, j;
    i = *((int *)arg);
    j = 1;
    sleep(10 + i);
    pthread_join(th2, NULL);
    printf("th1 is awake\n");
    pthread_exit(&j);
}

void *Func_th2(void *arg)
{
    int i, j;
    i = *((int *)arg);
    j = 2;
    sleep(20 + i);
    printf("th2 is awake\n");
    pthread_exit(&j);
}

int main(int argc, char *argv[])
{
    int i;

    pthread_attr_init(&atrib);
    printf("Pthread message:\n");
    i = rand(); // función que proporciona un número aleatorio
    pthread_create(&th1, &atrib, Func_th1, &i);
    pthread_create(&th2, &atrib, Func_th2, &i);

    printf("END\n");
    exit(0);
}
