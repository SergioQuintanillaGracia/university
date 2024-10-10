#include <errno.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
    int status;
    pid_t pid = fork();

    switch (pid) {
        case -1:
            printf("The child process couldn't be created\n");
            break;
        case 0:
            printf("[Child] PID: %d | PPID: %d\n", getpid(), getppid());
            sleep(5);
            printf("[Child] Finished\n");
        default:
            printf("[Parent] PID: %d | PPID: %d\n", getpid(), getppid());
            if (wait(&status) != -1) {
                printf("[Parent] Child ended correctly, status = %d\n", status);
            }
    }
}