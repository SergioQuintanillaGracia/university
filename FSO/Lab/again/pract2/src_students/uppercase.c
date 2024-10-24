#include <stdio.h> 
#define SIZE_STRING 200

int main() {
    // Character pointers to copy the input string
    char *p1, *p2;

    // A) Define the string variables string and string2
    char string[SIZE_STRING];
    char string2[SIZE_STRING];
    
    // B) Read string in the console 
    printf("Enter string: ");
    scanf("%[^\n]s", string);
    
    // C) Convert to uppercase
    p1 = string;
    p2 = string2;
    while (*p1 != '\0') {
        if (*p1 >= 97 && *p1 <= 122) {
            *p2 = *p1 - 32;
        } else {
            *p2 = *p1;
        }
        p1++;
        p2++;
    }
    *p2 = '\0';
    
    // D) Out in the console string2.
    printf("%s\n", string2);
}