#include <stdio.h>

void permute(int*, int*);
double abs(double);

int main() {
    int x = 10;
    int y = 20;
    printf("Before:   x: %d | y: %d\n", x, y);
    permute(&x, &y);
    printf("After:   x: %d | y: %d\n", x, y);

    int z = -20;
    printf("Before:   z: %d\n", z);
    z = abs(z);
    printf("After:   z: %d\n", z);
}

// Procedure, not function, as it doesn't return any value (void)
void permute(int *x, int *y) {
    int temp = *x;
    *x = *y;
    *y = temp;
}

double abs(double x) {
    return x < 0 ? -x : x;
}