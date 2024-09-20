#include <stdio.h>

int number;

int main() {
    int i;

    printf("Write a number: ");
    scanf("%d", &number);

    printf("The first %d natural numbers are:\n", number);
    for (i = 0; i < number; i++) {
        printf("%d\n", i);
    }

    printf("Bye\n");
    return 0;
}
