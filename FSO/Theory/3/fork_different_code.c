#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main() {
    pid_t val;
    int n = 0;

    printf("PID before fork(): %d\n", getpid());

    val = fork();

    if (val > 0) {
        printf("Parent PID: %d\n", getpid());
        n++;
    } else {
        printf("Child PID: %d\n", getpid());
    }

    printf("Process %d: var = %d\n", getpid(), n);
}