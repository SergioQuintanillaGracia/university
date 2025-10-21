#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
    pid_t pid = fork();

    switch (pid) {
        case -1:
            printf("Could not create fork\n");
            break;
        case 0:
            printf("[%d] Child with parent %d\n", getpid(), getppid());
            break;
        default:
            printf("[%d] Parent, I created %d\n", getpid(), pid);
    }
}