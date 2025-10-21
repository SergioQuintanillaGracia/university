#include <stdio.h>
#include <string.h>

int main() {
    char name[] = "Juan Pablo";
    char namecpy[20];

    printf("name: %s\n", name);

    strcpy(namecpy, name);

    printf("namecpy: %s\n", namecpy);
}