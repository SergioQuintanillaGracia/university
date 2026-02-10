#include <stdio.h>

int main() {
    #pragma omp parallel
    {
        for (int i = 0; i < 2; i++) {
            printf("%d\n", i);

            #pragma omp for
            for (int j = 0; j < 2; j++) {
                printf("# %d\n", j);
            }

            #pragma omp barrier
        }
    }
}