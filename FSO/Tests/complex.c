#include <stdio.h>

typedef struct {
    float real;
    float imag;
} Complex;

void printComplex(Complex *pc) {
    printf("%f + %fi\n", pc->real, pc->imag);
}

int main() {
    Complex c = (Complex){5, 2.22};
    Complex *pc = &c;

    pc->imag = 1.33;

    printf("Enter new i value: ");
    scanf("%f", &pc->imag);

    printComplex(pc);
}
