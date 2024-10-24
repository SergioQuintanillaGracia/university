#include <stdio.h>

#define N 10

int num;

int main() {
    int i;

    printf("Write a number: ");
    scanf("%d", &num);

    printf("The first %d natural numbers are:\n", num);
    for (i = 0; i < num; i++) {
        printf("%d\n", i);
    }

    printf("Bye\n");
    return 0;
}