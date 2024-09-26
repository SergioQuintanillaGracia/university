#include <stdio.h>

#define PI 3.1416

float areaC(float r) {
    return PI * r * r;
}

int main() {   
   float area, radius;
   radius = 10;   
   area = areaC(radius);   
   printf("Area of circle with radius %f is %f \n", radius, area); 
}