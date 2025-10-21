#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    printf("Parent process %d\n", getpid());
    fork();
    printf("I am %d process, my parent is %d\n", getpid(), getppid());
}