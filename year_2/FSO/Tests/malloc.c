#include <stdio.h>
#include <stdlib.h>

int main() {
    void *p;
    p = malloc(10000000000000);

    if (p == NULL) {
        printf("Error: Couldn't allocate memory\n");
    }
}