#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

# define NPROCESSES 4

int main() {
    pid_t pid[NPROCESSES];
    int status;

    for (int i = 0; i < NPROCESSES; i++) {
        pid[i] = fork();

        if (pid[i] == 0) {
            printf("[Child] PID: %d | PPID: %d\n", getpid(), getppid());
            sleep(3);
            exit(0);
        }
    }

    if (waitpid(pid[2], &status, 0) == pid[2]) {
        printf("[Parent] Third child finished\n");
    }
}