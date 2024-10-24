#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    int status, x;
    char *arguments[] = {"ls", "-R", "/", 0};

    pid_t childpid = fork();

    if (childpid == -1) {
        printf("Fork failed\n");
        exit(1);
    } else if (childpid == 0) {
        printf("Child running ls...\n");
        // if (execl("/bin/ls", "ls", "-1", NULL) < 0) {
        if (execvp(arguments[0], arguments)) {
            printf("Couldn't run ls\n");
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