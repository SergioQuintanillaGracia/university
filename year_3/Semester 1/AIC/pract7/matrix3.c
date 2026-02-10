#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#define NMIN 256
#define NMAX 4096

#define BLOCKSIZE 256

void multMat_blocking( int n, float *A, float *B, float *C ) {
    int i, j, k;
    int bi, bk;
    /* ikj */
    for (bi = 0; bi < n; bi += BLOCKSIZE)
        for (bk = 0; bk < n; bk += BLOCKSIZE)
            for (i = bi; i < bi + BLOCKSIZE; i++)
                for (k = bk; k < bk + BLOCKSIZE; k++)
                    for (j = 0; j < n; j++)
                        C[i * n + j] += A[k + i * n] * B[j + k * n];
}

void multMat( int n, float *A, float *B, float *C ) {
    int i,j,k;
    /* ikj */
    for( i = 0; i < n; i++ )
        for( k = 0; k < n; k++ )
            for( j = 0; j < n; j++ )
                C[i*n+j] += A[k+i*n]*B[j+k*n];
}

int main( int argc, char **argv ) {
    int i,n;

    float *A = (float *)malloc( NMAX*NMAX * sizeof(float));
    float *B = (float *)malloc( NMAX*NMAX * sizeof(float));
    float *C = (float *)malloc( NMAX*NMAX * sizeof(float));

    /* comprueba que multMat_blocking funciona correctamente */
    {
        float *Ct = (float *)malloc( NMAX*NMAX * sizeof(float));

	n = 2*BLOCKSIZE; // Probar con matrices de al menos 2 bloques de ancho. Hay alguna implementación errónea de alumnos que solo itera en un bloque.
	
        /* rellena A y B con números aleatorios */
        for( i = 0; i < n*n; i++ ) A[i] = drand48()*2-1;
        for( i = 0; i < n*n; i++ ) B[i] = drand48()*2-1;

        multMat_blocking( n, A, B, C );
        multMat( n, A, B, Ct );

        for( i = 0; i < n*n; i++ )
            if ( C[i] != Ct[i] ) {
                printf("Error!!!! El resultado NO coincide con multMat!!\n");
                exit( -1 );
            }
    }
                
    struct timeval start, end;

    for( n = NMAX; n >= NMIN; n >>= 1 ) {
        /* rellena las matrices con números aleatorios */
        for( i = 0; i < n*n; i++ ) A[i] = drand48()*2-1;
        for( i = 0; i < n*n; i++ ) B[i] = drand48()*2-1;
        for( i = 0; i < n*n; i++ ) C[i] = drand48()*2-1;

        /* multiplica las matrices y mide el tiempo */
        gettimeofday( &start, NULL );
        multMat( n, A, B, C );
        gettimeofday( &end, NULL );

        /* calcula Gflops */
        double seconds = (end.tv_sec - start.tv_sec) + 1.0e-6 * (end.tv_usec - start.tv_usec);
        double Gflops = 2e-9*n*n*n/seconds;    
        printf( "non-blocking n = %d, %.3f Gflop/s\n", n, Gflops );

        /* rellena las matrices con números aleatorios */
        for( i = 0; i < n*n; i++ ) A[i] = drand48()*2-1;
        for( i = 0; i < n*n; i++ ) B[i] = drand48()*2-1;
        for( i = 0; i < n*n; i++ ) C[i] = drand48()*2-1;

        /* multiplica las matrices y mide el tiempo */
        gettimeofday( &start, NULL );
        multMat_blocking( n, A, B, C );
        gettimeofday( &end, NULL );

        /* calcula Gflops */
        seconds = (end.tv_sec - start.tv_sec) + 1.0e-6 * (end.tv_usec - start.tv_usec);
        Gflops = 2e-9*n*n*n/seconds;    
        printf( "    blocking n = %d, %.3f Gflop/s\n", n, Gflops );
    }

    free( A );
    free( B );
    free( C );

    printf("\n\n");

    return 0;
}
