#include <stdio.h>

#define N 10

int number;

int main() {
    int i;

    number = N;

    printf("The first %d natural numbers are:\n", number);
    for (i = 0; i < number; i++) {
        printf("%d\n", i);
    }

    printf("Bye\n");
    return 0;
}
