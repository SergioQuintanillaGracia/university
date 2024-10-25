#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    pid_t childpid;
    int status, childreturnpid;
    char **arguments = argv + 1;

    childpid = fork();

    if (childpid == -1) {
        printf("Fork failed\n");
        exit(1);
    } else if (childpid == 0) {
        printf("Running command\n");
        if (execvp(argv[1], arguments) < 0) {
            printf("Could not execute command\n");
            exit(1);
        }
    }

    childreturnpid = wait(&status);

    if (childreturnpid != childpid) {
        printf("Child has been interrupted by a signal\n");
    }

    exit(0);
}