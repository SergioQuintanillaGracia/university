#include <stdio.h>

int main() {
    int b;
    int x = 12;
    int *p;
    int n[3] = {1, 2, 3};
    p = &x;
    b = *p;
    *p = 10;
    p = n;

    printf("b: %d | x: %d | p[1]: %d\n", b, x, p[1]);
    printf("&b: %d | x: %d\n", &b, &x);
    printf("After the value b in memory, there is this value: "
            "%d (in address %d)\n", *(&b - 1), &b - 1);
    // NOTE: In many systems, the stack grows downwards, which means that
    // to get to a variable `a` defined **before** `b`, you may have to do `&b - 1` 
    // to get to `a`, because `a` has a higher memory address than `b`.
    // On other systems, where the stack grows upwards, you may need to do `&a + 1` 
    // to get to `b`, because `b` would have a higher memory address than `a`.
}