#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_PROC 4

int main() {
    int i;
    int finalState;

    for (i = 0; i < MAX_PROC; i++) {
        pid_t valReturn = fork();

        if (valReturn == 0) {
            printf("Child %d created in iteration %d\n", getpid(), i);
        } else {
            printf("Parent %d, iteration %d | I created %d\n", getpid(), i, valReturn);
            break;
        }
    }

    while (wait(&finalState) > 0) {
        printf("Parent %d iteration %d\n", getpid(), i);
        printf("My child said %d\n", WEXITSTATUS(finalState));
        printf("My child said %d\n", finalState / 256);
    }

    exit(i);
}
