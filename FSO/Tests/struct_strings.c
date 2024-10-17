#include <stdio.h>
#include <string.h>

typedef struct {
    char text[20];
} Text;

int main() {
    Text t = (Text){"Hello"};
    printf("%s\n", t.text);

    strcpy(t.text, "Bye bye");
    printf("%s\n", t.text);
}