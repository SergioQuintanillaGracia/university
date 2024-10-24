#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    int stat_loc;

    for (int i = 0; i < 5; i++) {
        int f = fork();

        if (f == 0) {
            printf("%d child created\n", i);
            sleep(5);
            exit(i);
        }
    }

    for (int i = 0; i < 5; i++) {
        int pid = wait(&stat_loc);
        printf("Child %d state received: %d\n", pid, WEXITSTATUS(stat_loc));
    }
}