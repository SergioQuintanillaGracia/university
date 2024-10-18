#include <stdio.h>

#define E 2.7182
#define ADD(a, b) a + b
#define MAX(a, b) a > b ? a : b
#define TIMES(t, instruc) for (int i = 0; i < t; i++) { instruc; }

int main() {
    printf("%f\n", E);
    printf("%d\n", ADD(1, 2));
    printf("%d\n", MAX(2, 6));
    TIMES(10, {
        printf("a\n");
        printf("b\n");
    });

    #if defined(E)
    printf("E is defined\n");
    #elif !defined(E)  // Better to put `#else`
    printf("E is not defined\n");
    #endif
}
