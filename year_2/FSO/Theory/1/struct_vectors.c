#include <stdio.h>

struct CD {
    char title[100];
    char artist[50];
    int numSongs;
    int year;
    float price;
};

int main() {
    struct CD arr[100];
    struct CD *ptr;

    for (int i = 0; i < 100; i++) {
        arr[i].price = 12.5;
    }

    ptr = &arr[10];
    ptr->price = 16;

    printf("Price of arr[10]: %f\n", arr[10].price);
}