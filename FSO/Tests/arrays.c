#include <stdio.h>
#include <string.h>

void printArr(int *a, int size) {
    printf("{");
    
    for (int i = 0; i < size; i++) {
        printf("%d", a[i]);

        if (i < size - 1) {
            printf(", ");
        }
    }

    printf("}\n");
}

int main() {
    int a[5] = {1, 2, 3, 4, 5};
    int b[5];

    printArr(a, 5);
    printArr(b, 5);

    // sizeof(a) returns the number of bytes the array a
    // takes, which is what memcpy expects
    memcpy(b, a, sizeof(a));  

    printArr(a, 5);
    printArr(b, 5);
}