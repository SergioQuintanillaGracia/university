#include <stdio.h>

#define E 2.7182
#define g 9.81
#define ADD(a, b) (a + b)
#define MAX(a, b) (a > b) ? a : b
// Definitions can be removed with #undef MACRO
// Ex: `#undef MAX`

int main() {
    printf("E + g = %f\n", ADD(E, g));
    printf("MAX(E, g) = %f\n", MAX(E, g));
}