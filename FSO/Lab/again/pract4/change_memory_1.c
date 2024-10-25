#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    pid_t childpid;
    int status, childreturnpid;
    char *arguments[] = {"ls", "-R", "/", NULL};

    childpid = fork();

    if (childpid == -1) {
        printf("Fork failed\n");
        exit(1);
    } else if (childpid == 0) {
        printf("Running ls\n");
        if (execv("/bin/ls", arguments) < 0) {
            printf("Could not execute ls\n");
            exit(1);
        }
    }

    childreturnpid = wait(&status);

    if (childreturnpid != childpid) {
        printf("Child has been interrupted by a signal\n");
    }

    exit(0);
}