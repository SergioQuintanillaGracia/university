#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

#define NPROC 4

int main() {
    pid_t pid;
    int i;

    for (i = 0; i < NPROC; i++) {
        pid = fork();

        if (pid != 0) {
            // Parent process
            break;
        }

        printf("[%d] Child process with parent %d\n", getpid(), getppid());
    }
}