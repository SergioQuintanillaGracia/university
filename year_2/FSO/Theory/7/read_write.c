#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s from_file to_file\n", argv[0]);
        return 1;
    }

    char fnamein[strlen(argv[1])];
    char fnameout[strlen(argv[2])];
    strcpy(fnamein, argv[1]);
    strcpy(fnameout, argv[2]);

    int fdin = open(fnamein, O_RDONLY);
    int fdout = open(fnameout, O_WRONLY | O_CREAT, 0600);

    if (fdin == -1) {
        fprintf(stderr, "Could not open %s: %s\n", fnamein, strerror(errno));
        return 1;
    }

    if (fdout == -1) {
        fprintf(stderr, "Could not open %s: %s\n", fnameout, strerror(errno));
        return 1;
    }

    char buf[1024];
    int nreads = 0;
    int nbytes = 0;
    int count = 0;

    while ((count = read(fdin, buf, sizeof(buf))) > 0) {
        nreads++;
        nbytes += count;
        write(fdout, buf, count);
    }

    printf("Copied %s to %s\n", fnamein, fnameout);
    printf("Reads: %d  |  Bytes read: %d\n", nreads, nbytes);

    return 0;
}
