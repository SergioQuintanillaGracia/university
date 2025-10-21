#include <stdio.h>

#define PRINT_BYE

int main() {
    // Can be replaced by `#ifdef PRINT_HELLO`
    // The opposite can be achieved by using `#ifndef PRINT_HELLO`
    // or `#if !defined(PRINT_HELLO)`
    #if defined(PRINT_HELLO)
    printf("Hello\n");

    #elif defined(PRINT_BYE)
    printf("Bye\n");

    #else
    printf("Nothing to print\n");

    #endif
}