#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) {
    int currArg = 1;

    for (int i = 1; i < argc; i++) {
        printf("Argument %d is ", currArg);

        if (strlen(argv[i]) != 2) {
            printf("Unknown\n");
        } else if (argv[i][0] == '-' && argv[i][1] == 'c') {
            printf("Compile\n");
        } else if (argv[i][0] == '-' && argv[i][1] == 'E') {
            printf("Preprocess\n");
        } else if (argv[i][0] == '-' && argv[i][1] == 'i') {
            printf("Include ");

            // Move to the next argument, which contains
            // the folder to include
            i++;
            printf("%s\n", argv[i]);
        } else {
            printf("Unknown\n");
        }

        currArg++;
    }
}
