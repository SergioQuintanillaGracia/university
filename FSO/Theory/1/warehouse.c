#include <stdio.h>
#include <string.h>
#define NUMBOXES 3

typedef struct {
    char part[20];  // Part type
    int quantity;  // Part count
    float unitPrice;  // Part price
    char available;  // If there are parts
} PartsRecord;

void printPartsRecord(PartsRecord *ppr) {
    printf("PartsRecord{part: %s, quantity: %d, unitPrice: %f, available: %c}\n",
            ppr->part, ppr->quantity, ppr->unitPrice, ppr->available);
}

int main() {
    PartsRecord boxes[NUMBOXES];
    int count = 0;

    do {
        printf("Enter part name: ");
        scanf("%s", boxes[count].part);
        printf("Enter number of parts: ");
        scanf("%d", &boxes[count].quantity);
        printf("Enter unit price: ");
        scanf("%f", &boxes[count].unitPrice);
        boxes[count].available = 'V';
    } while (++count < NUMBOXES);

    for (int i = 0; i < sizeof(boxes) / sizeof(boxes[0]); i++) {
        printPartsRecord(&boxes[i]);
    }
}