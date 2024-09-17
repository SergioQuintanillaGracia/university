#include <stdio.h>

int main() {
    void *v;  // Generic pointer
    int i[3] = {10, 20, 30};
    int a;
    v = i;  // v now points to an int (the first value in i)
    a = *(int*) v;  // We have to cast the pointer to int because if we don't,
                    // the compiler won't know how many bytes to read or
                    // interpret when referencing it
    printf("a = %d\n", a);
}