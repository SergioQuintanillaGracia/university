#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

#define NPROCESSES 4

int main() {
    pid_t pid;
    
    for (int i = 0; i < NPROCESSES; i++) {
        pid = fork();

        if (pid != 0) {
            // Parent process
            // break;
        }

        printf("Child process with PID: %d and PPID: %d\n", getpid(), getppid());
        sleep(0.1);
    }

    sleep(5);
}