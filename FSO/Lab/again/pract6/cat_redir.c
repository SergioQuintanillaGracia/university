#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

int main() {
    int fd;
    char *file = "ls_output.txt";
    mode_t fd_mode = S_IRWXU;

    fd = open(file, O_RDWR | O_CREAT, fd_mode);

    if (dup2(fd, STDIN_FILENO) == -1) {
        printf("Error calling dup2\n");
        exit(-1);
    }

    execlp("cat", "cat", NULL);

    return 0;
}