#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#define NMIN 64
#define NMAX 2048

void multMat_ikj( int n, float *A, float *B, float *C ) {
    int i,j,k;
    /* ikj */
    for (i = 0; i < n; i++)
        for (k = 0; k < n; k++)
            for (j = 0; j < n; j++)
                C[i * n + j] += A[k + i * n] * B[j + k * n];
}

int main( int argc, char **argv ) {
    int i,n;

    float *A = (float *)malloc( NMAX*NMAX * sizeof(float));
    float *B = (float *)malloc( NMAX*NMAX * sizeof(float));
    float *C = (float *)malloc( NMAX*NMAX * sizeof(float));

    struct timeval start, end;

    for( n = NMAX; n >= NMIN; n >>= 1 ) {
        /* rellena las matrices con números aleatorios */
        for( i = 0; i < n*n; i++ ) A[i] = drand48()*2-1;
        for( i = 0; i < n*n; i++ ) B[i] = drand48()*2-1;
        for( i = 0; i < n*n; i++ ) C[i] = drand48()*2-1;

        /* multiplica las matrices y mide el tiempo */
        gettimeofday( &start, NULL );
        multMat_ikj( n, A, B, C );
        gettimeofday( &end, NULL );

        /* calcula Gflops */
        double seconds = (end.tv_sec - start.tv_sec) + 1.0e-6 * (end.tv_usec - start.tv_usec);
        double Gflops = 2e-9*n*n*n/seconds;    
        printf( "n = %d, %.3f Gflop/s\n", n, Gflops );
    }

    free( A );
    free( B );
    free( C );

    printf("\n\n");

    return 0;
}
