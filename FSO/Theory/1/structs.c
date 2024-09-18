#include <stdio.h>
#include <string.h>

struct CD {
    char title[100];
    char artist[50];
    int numSongs;
    int year;
    float price;
};

void printCdPrice(struct CD *ptr) {
    printf("CD Price: %f\n", ptr->price);
}

int main() {
    struct CD cd1 = {"Title", "Artist", 5, 2001, 19.99};
    // To assign other values to cd1, you can do it this way:
    cd1 = (struct CD){"NewTitle", "NewArtist", 3, 2002, 16.99};

    struct CD cd2;
    strcpy(cd2.title, "Title 2");
    strcpy(cd2.artist, "Artist 2");
    cd2.numSongs = 20;
    cd2.year = 2004;

    struct CD *pcd;
    pcd = &cd1;
    printf("cd1 title through pointer: %s\n", pcd->title);
    pcd->price = 0.99;
    printf("cd1 price modified through pointer: %f\n", cd1.price);

    printCdPrice(&cd1);
}
