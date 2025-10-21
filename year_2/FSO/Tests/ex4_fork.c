#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
    pid_t val;
    int var = 0;

    printf("PID Before fork: %d\n", getpid());
    val = fork();

    if (val > 0) {
        printf("Parent PID: %d\n", getpid());
        var++;
    } else {
        printf("Child PID: %d\n", getpid());
    }

    printf("Process [%d], var: %d\n", getpid(), var);
}