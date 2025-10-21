#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>

int main() {
    int status;
    pid_t pid = fork();

    switch (pid) {
        case -1:
            printf("Error creating child\n");
            break;
        case 0:
            printf("[%d] Child process, working...\n", getpid());
            sleep(3);
            printf("[%d] Finished\n", getpid());
            break;
        default:
            printf("[%d] Parent with child %d, waiting...\n", getpid(), pid);
            if (wait(&status) != -1) {
                printf("My child ended ok with status %d\n", WEXITSTATUS(status));
            }
    }
}