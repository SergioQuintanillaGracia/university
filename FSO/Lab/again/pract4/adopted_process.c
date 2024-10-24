#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    for (int i = 0; i < 5; i++) {
        int f = fork();

        if (f == 0) {
            printf("%d child created\n", i);
            sleep(10);
            exit(i);
        }
    }

    sleep(5);
}