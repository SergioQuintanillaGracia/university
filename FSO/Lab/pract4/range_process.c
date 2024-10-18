#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    for (int i = 0; i < 5; i++) {
        int fret = fork();

        if (fret == 0) {
            printf("Child created in iteration %d\n", i + 1);
            exit(i);
        }
    }

    sleep(10);
}