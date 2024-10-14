#include <stdio.h>
#include <string.h>

struct CD {
    char title[100];
    char artist[50];
    float price;
};

int main() {
    struct CD cd1;
    strcpy(cd1.title, "CD Title");
    strcpy(cd1.artist, "Epic artist");
    cd1.price = 100;

    struct CD cd2 = (struct CD){"JuanTitle", "ArtistaEpico", 12893};
    
    printf("Title: %s\nArtist: %s\nPrice: %f\n", cd1.title, cd1.artist, cd1.price);
    printf("Title: %s\nArtist: %s\nPrice: %f\n", cd2.title, cd2.artist, cd2.price);
}