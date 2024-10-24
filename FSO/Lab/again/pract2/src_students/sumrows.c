#include <stdio.h> 
#define SIZE_ROW 100
#define NUM_ROWS 10

struct ROW {
    float data[SIZE_ROW];
    float addition;
};

// A) define a vector “rows” of structures ROW with size NUM_ROWS
struct ROW rows[NUM_ROWS];
	
void sum_row (struct ROW *pf) {
    pf->addition = 0;
    for (int i = 0; i < SIZE_ROW; i++) {
        pf->addition += pf->data[i];
    }
}

// Initilize rows with value i * j
void init_rows() {
    int i, j;
    for (i = 0; i < NUM_ROWS; i++) {
        for (j = 0; j < SIZE_ROW; j++) {
            rows[i].data[j] = (float)i*j;
        }
    }
}

int main() { 
    int i;
    float total_sum;
    
    init_rows ();
    
    // C) Complete the loop
    total_sum = 0;
    for (i = 0; i < NUM_ROWS; i++) {
        sum_row(&(rows[i]));
        printf("Row %u addition result is %f\n", i, rows[i].addition);
        total_sum += rows[i].addition;
    }

    printf("Final addition result is %f\n", total_sum); 
} 
