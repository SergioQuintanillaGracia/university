#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main() {
    pid_t pid = fork();

    // The last element of the args array must be `NULL`, so `execvp`
    // knows when to stop reading arguments
    char *args[] = {"ls", "-l", NULL};

    switch (pid) {
        case -1:
            printf("Could not create child process\n");
            break;
        
        case 0:
            printf("Child with PID %d, current dir content is:\n", getpid());
            if (execvp("ls", args) == -1) {
                printf("Error executing ls\n");
                return 0;
            }
            break;
        
        default:
            printf("Parent process with PID %d, my child is %d\n", getpid(), pid);
    }
}