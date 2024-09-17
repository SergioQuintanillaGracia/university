#include <stdio.h>

void printa();

int a = 10;  // Global variable

int main() {
    printa();
    a++;
    printa();
    a++;
    printa();
}

void printa() {
    static int callCount = 1;
    printf("a: %d | Called %d time(s)\n", a, callCount);
    callCount++;
}