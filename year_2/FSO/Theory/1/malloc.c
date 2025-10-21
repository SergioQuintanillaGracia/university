#include <stdio.h>
#include <stdlib.h>  // For malloc and free

int main() {
    // Dynamically allocate memory for an array of 5 ints
    // `malloc` returns a void pointer, so we have to cast it to an int pointer
    int *arr = (int*) malloc(5 * sizeof(int));

    // Check if memory could be allocated
    if (arr == NULL) {
        printf("Memory allocation failed\n");
        return 1;  // Exit the program
    }

    // Initialize the array
    for (int i = 0; i < 5; i++) {
        arr[i] = i + 1;
        printf("arr[%d] set to %d\n", i, arr[i]);
    }

    // Free the dynamically allocated memory
    // When the program terminates, the OS will automatically reclaim the memory
    // it was using, even if free is not called (in most cases)
    free(arr);

    return 0;
}