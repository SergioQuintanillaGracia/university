#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>

int main() {
    int status;
    pid_t pid = fork();

    char *args[] = {"ls", "-l", NULL};

    switch (pid) {
        case -1:
            printf("Couldn't create fork\n");
            break;
        case 0:
            printf("[%d] Child process, the current directory content is:\n", getpid());
            if (execvp(args[0], args) == -1) {
                printf("Error running ls\n");
                exit(0);
            }
            break;
        default:
            printf("[%d] Parent process with child %d\n", getpid(), pid);
    }
}