#include <stdio.h>
#include <unistd.h>

int main() {
    printf("PID: %ld\n", (long) getpid());
    printf("Parent PID %ld\n", (long) getppid());
    while(1);
}