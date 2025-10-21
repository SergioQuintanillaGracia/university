#include <stdio.h>
#include <string.h>

int main() {
    char s1[20] = "Hello";
    char *s2 = " World";

    printf("%s\n", s1);
    strcat(s1, s2);
    printf("%s\n", s1);

    printf("Length: %d\n", strlen(s1));

    printf("strcmp(s1, s2): %d\n", strcmp(s1, s2));
}
