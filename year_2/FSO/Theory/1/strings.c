#include <stdio.h>
#include <string.h>

int main() {
    char s1[30] = "Hello";  // We need to allocate space for more characters
                            // so we can append `s2` to the end of `s1`
    char s2[] = " world";  // Can be replaced by `char *s2 = ...;`
    
    // Concatenates `s2` to the end of `s1`, returning the address of `s1`
    // `s1` now contains the concatenated string
    strcat(s1, s2);
    printf("Concatenated string: %s\n", s1);

    // Copy `s2` to `s1`, overwriting `s1`
    strcpy(s1, s2);
    printf("Overwritten string: %s\n", s1);

    // Copy a string to the char array
    // `s1 = "Hello"` raises an error, as `s1` represents the address of the first
    // element of the char array. It's not a "normal" pointer, it always points
    // to the starting location of the array in memory. It cannot be reasigned to
    // point somewhere else (it can't be reassigned to point to the memory location
    // where the string literal `"Hello"` is stored).
    // To get "Hello" into `s1`, we have to copy the contents of the string literal
    // `"Hello"` into the memory that `s1` points to. The place in memory where
    // `s1` points to doesn't change.
    strcpy(s1, "Hello");

    // Get the length of `s1`
    int s1Len = strlen(s1);
    printf("s1 length: %d\n", s1Len);

    // Compare 2 strings
    strcpy(s1, "abc");
    strcpy(s2, "cde");
    // > 0 if s1 > s2
    // = 0 if s1 == s2
    // < 0 if s2 > s1
    printf("Comparison result: %d\n", strcmp(s1, s2));
}