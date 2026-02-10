#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <xmmintrin.h>
#include <immintrin.h>

#define MSIZE 2048 /* Must be multiple of 4 */

//#define LOAD
//#define SCALAR
//#define SSE
//#define AVX
//#define AVX512

float vector_in[MSIZE], vector_in2[MSIZE];

//////////
// CODE //
//////////

void init_vector(float *t, int size) {
 
  int i;
  float num, dnum;
  
  num = rand()/(RAND_MAX+1.0);
  dnum = rand()/(RAND_MAX+1.0);
  for(i=0; i<size; i++) {
    t[i] = num;
    num += dnum;
  } // end for
 
} // end init_vector()

#if defined(SSE)

float ScalarSSE(float *m1, float *m2, int size) {
 
    float prod = 0.0;
    int i;
    __m128 X, Y, Z;
    
    Z = _mm_setzero_ps(); /* Z needs to be initialized to all 0.0 */
    for(i=0; i<size; i+=4) {
      X = _mm_load_ps(&m1[i]);
      Y = _mm_load_ps(&m2[i]);
      X = _mm_mul_ps(X, Y);
      Z = _mm_add_ps(X, Z);
    }
    
    for(i=0; i<4; i++) {
      prod += Z[i];
    }
 
    return prod;
 
} // end ScalarSSE()

#elif defined(AVX) 

float ScalarAVX(float *m1, float *m2, int size) {
 
    float prod = 0.0;
    int i;
    __m256 X, Y, Z;
    
    Z = _mm256_setzero_ps(); // Z needs to be initialized to all 0.0 
    for(i=0; i<size; i+=8) {
      X = _mm256_load_ps(&m1[i]);
      Y = _mm256_load_ps(&m2[i]);
      X = _mm256_mul_ps(X, Y);
      Z = _mm256_add_ps(X, Z);
    }
    
    for(i=0; i<8; i++) {
      prod += Z[i];
    }
 
    return prod;
 
} // end ScalarAVX()

#elif defined(AVX512)

float ScalarAVX512(float *m1, float *m2, int size) {
 
    float prod = 0.0;
    int i;
    __m512 X, Y, Z;
    
    Z = _mm512_setzero_ps(); // Z needs to be initialized to all 0.0 
    for(i=0; i<size; i+=16) {
      X = _mm512_load_ps(&m1[i]);
      Y = _mm512_load_ps(&m2[i]);
      X = _mm512_mul_ps(X, Y);
      Z = _mm512_add_ps(X, Z);
    }
    
    for(i=0; i<16; i++) {
      prod += Z[i];
    }
 
    return prod;
 
} // end ScalarAVX512()

#else
 
float Scalar(float *s1, float *s2, int size) {
 
    int i;
    float prod = 0.0;
 
    for(i=0; i<size; i++) {
      prod += s1[i] * s2[i];
    }
 
    return prod;
 
} // end Scalar()

#endif

int main(int argc, char * argv[]) {
 
    int     i;
    int     rep=10;
    int     msize=MSIZE;
    float   fvalue;

    if (argc == 2) { 
      rep = atoi(argv[1]);
    } else if (argc == 3) {
      rep = atoi(argv[1]);
      msize = atoi(argv[2]);
      msize = (msize > MSIZE) ? MSIZE : msize;
    } // end if/else

   fprintf(stderr, "\n"); 

   fprintf(stderr, "Rep = %d / size = %d\n", rep, msize);
    
    for(i=0; i<rep; i++) {
      init_vector(vector_in, msize);
      init_vector(vector_in2, msize);
#if defined(LOAD)
      // DO NOTHING
#elif defined(SSE)
      fvalue = ScalarSSE(vector_in2, vector_in, msize);
#elif defined(AVX)
      fvalue = ScalarAVX(vector_in2, vector_in, msize);
#elif defined(AVX512)
      fvalue = ScalarAVX512(vector_in2, vector_in, msize);
#elif defined(SCALAR)
      fvalue = Scalar(vector_in2, vector_in, msize);  
#else 
#error " * * * Instruction set macro unknown! * * * "
#endif
    } // end for

    exit(0);
    
} // end main()
