#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    int status, x;

    pid_t childpid = fork();

    if (childpid == -1) {
        printf("Fork failed\n");
        exit(1);
    } else if (childpid == 0) {
        printf("Child running command...\n");
        // By passing as arguments the address of the first argv element,
        // which is the name of the command (ex: ls / ps), C will
        // continue searching for arguments until it finds NULL
        // argv always ends with NULL, so it will take all the arguments
        // in argv from the name of the command to the last one
        // (ex: ps aux)
        if (execvp(argv[1], &argv[1])) {
            printf("Couldn't run command\n");
            exit(1);
        }
        printf("This will not be printed\n");
    }

    x = wait(&status);
    if (x != childpid) {
        printf("Child was interrupted by a signal\n");
    }

    exit(0);
}