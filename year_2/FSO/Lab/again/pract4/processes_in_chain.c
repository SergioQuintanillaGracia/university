#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_PROC 4

int main() {
    int i;
    pid_t val_return;
    int final_state;

    for (i = 0; i < MAX_PROC; i++) {
        val_return = fork();

        if (val_return == 0) {
            printf("Child %d created in iteration %d\n", getpid(), i);
        } else {
            printf("Parent %d, iteration %d, I created %d\n", getpid(), i, val_return);
            break;
        }
    }

    while (wait(&final_state) > 0) {
        printf("Parent %d, iteration %d, my child said %d\n", getpid(), i, WEXITSTATUS(final_state));
    }

    exit(i);
}