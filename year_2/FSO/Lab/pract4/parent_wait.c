#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    for (int i = 0; i < 5; i++) {
        int fret = fork();

        if (fret == 0) {
            printf("Child created in iteration %d\n", i + 1);
            sleep(20);
            exit(i);
        }
    }

    sleep(10);

    for (int i = 0; i < 5; i++) {
        wait(NULL);
        printf("Child process state received\n");
    }
}
