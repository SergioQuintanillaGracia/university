#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    int n1, n2;

    if (argc == 3) {
        n1 = atoi(argv[1]);
        n2 = atoi(argv[2]);
        printf("%d + %d = %d\n", n1, n2, n1 + n2);
    } else {
        printf("Command usage: %s n1 n2\n", argv[0]);
    }
}