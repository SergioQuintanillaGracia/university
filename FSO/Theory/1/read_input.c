#include <stdio.h>
#include <math.h>

void readCatheti(double *a, double *b) {
    printf("Enter a and b catheti values: ");
    scanf("%lf %lf", a, b);
}

void hypothenuse(double a, double b, double *h) {
    *h = sqrt(pow(a, 2) + pow(b, 2));
}

int main() {
    double a, b, h;
    
    readCatheti(&a, &b);
    hypothenuse(a, b, &h);

    printf("Hypothenuse for a = %lf and b = %lf: %lf\n", a, b, h);
}