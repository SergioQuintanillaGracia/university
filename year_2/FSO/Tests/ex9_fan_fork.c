#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>

#define NPROC 4

int main() {
    pid_t pid[NPROC];
    int i;
    int status;

    for (i = 0; i < NPROC; i++) {
        pid[i] = fork();

        if (pid[i] == 0) {
            printf("[%d] Child with parent %d\n", getpid(), getppid());
            sleep(2);
            exit(0);
        }
    }

    for (i = 0; i < NPROC; i++) {
        if (wait(&status) != -1) {
            printf("Child ended with status %d\n", WEXITSTATUS(status));
        }
    }
}