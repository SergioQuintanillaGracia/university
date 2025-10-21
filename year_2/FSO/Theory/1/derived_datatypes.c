#include <stdio.h>

struct Complex {
    float real;
    float imag;
};

void printComplex(struct Complex *pc) {
    printf("r: %.2f i: %.2f\n", pc->real, pc->imag);
}

int main() {
    struct Complex c1, c2[10];

    c1.real = 10.1;
    c1.imag = -2.3;

    for (int i = 0; i < 10; i++) {
        c2[i].real = i + (float) i / 10;
        c2[i].imag = i - (float) i / 10;
    }

    printComplex(&c1);

    for (int i = 0; i < 10; i++) {
        printComplex(&c2[i]);
    }

    printf("Size of float = %lu\n", sizeof(float));
    printf("Size of c1 = %lu\n", sizeof(c1));
    printf("Size of c2 = %lu\n", sizeof(c2));
    printf("Amount of elements in c2 = %lu\n", sizeof(c2) / sizeof(struct Complex));
}