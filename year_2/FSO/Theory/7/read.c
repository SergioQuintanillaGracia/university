#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
    char* fname = argv[1];
    int fdin = open(fname, O_RDONLY);

    if (fdin == -1) {
        fprintf(stderr, "Could not open %s: %s\n", fname, strerror(errno));
        exit(1);
    }

    char buf[20];
    int nreads = 0;
    int nbytes = 0;
    int count = 0;

    while ((count = read(fdin, buf, sizeof(buf))) > 0) {
        nreads++;
        nbytes += count;
    }

    printf("Reads: %d  |  Bytes read: %d\n", nreads, nbytes);

    close(fdin);

    return 0;
}