#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) {
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-c") == 0) {
            printf("Argument %d is Compile\n", i);
        } else if (strcmp(argv[i], "-E") == 0) {
            printf("Argument %d is Preprocess\n", i);
        } else if (strcmp(argv[i], "-i") == 0) {
            printf("Argument %d is Include %s\n", i, argv[i + 1]);
            i++;
        }
    }
}