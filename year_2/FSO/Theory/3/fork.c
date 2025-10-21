#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main() {
    printf("Process %ld creates another process\n", (long) getpid());
    
    // The child process is an exact copy of its parent
    // The first instruction it will execute is the assignment of `fork` to `res`
    // `res` will be 0 for the child process
    int res = fork();
    printf("Process %ld with parent %ld\n", (long) getpid(), (long) getppid());
    printf("res = %d\n", res);
}