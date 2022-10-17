#include <stdlib.h>
#include <stdio.h>
#include <x86intrin.h>
#include <sys/time.h>
#define ARRAY_SIZE 2048
//#define NUMBER_OF_TRIALS 100000
extern __m128 _mm_add_ps( __m128 _A, __m128 _B );
/*
 * Statically allocate our arrays.  Compilers can
 * align them correctly.
 */
static double a[ARRAY_SIZE], b[ARRAY_SIZE], c;
__m256d bb = {0, 1, 2, 3};
__m256d ab = {1, 2, 3, 4};
__m256d vi = {0, 0, 0, 0};
struct timeval start, end;

int main(int argc, char *argv[]) {
    int i,t;

    double m = 1.0001;

    /* Populate A and B arrays */

    for (int i=0; i < ARRAY_SIZE; i+=4) {
        //cast i to a double
        double di = (double) i;
        vi = _mm256_broadcast_sd(&di);        
        
        // store bb and ab in b and a
        _mm256_storeu_pd(&b[i], _mm256_add_pd(bb, vi));
        _mm256_storeu_pd(&a[i], _mm256_add_pd(ab, vi));
    }
    // for (i=0; i < ARRAY_SIZE; i++) {
    //     b[i] = i;
    //     a[i] = i+1;
    // }


    for (int x=100000; x<=1000000; x+=100000)
    {
        int NUMBER_OF_TRIALS = x;

        // get current system time
        gettimeofday(&start, NULL);

        __m256d mm = {1.0001, 1.0001, 1.0001, 1.0001};
        __m256d sum = {0.0, 0.0, 0.0, 0.0}; // to hold partial sums
        /* Perform an operation a number of times */
        for (t=0; t < NUMBER_OF_TRIALS; t++) {
            for (i=0; i < ARRAY_SIZE; i+=4) {
                // load arrays
                __m256d va = _mm256_load_pd(&a[i]);
                __m256d vb = _mm256_load_pd(&b[i]);
                // compute m*a + b
                __m256d tmp = _mm256_fmadd_pd(mm, va, vb);
                // accumulate partial sums
                sum = _mm256_add_pd(tmp, sum);
            }
        }
        // Get sum[2], sum[3]
        __m128d xmm = _mm256_extractf128_pd (sum, 1);   // Extract 128 bits (composed of 2 packed double-precision (64-bit) floating-point elements) from a, selected with imm8.
        // Extend to 256 bits: sum[2], sum[3], 0, 0
        __m256d ymm = _mm256_castpd128_pd256(xmm);      // Cast vector of type __m128d to type __m256d; the upper 128 bits of the result are undefined.
        // Perform sum[0]+sum[1], sum[2]+sum[3], sum[2]+sum[3], 0+0
        sum = _mm256_hadd_pd (sum, ymm);                // Horizontally add adjacent pairs of double-precision (64-bit) floating-point elements in a and b.
        // Perform sum[0]+sum[1]+sum[2]+sum[3]…
        sum = _mm256_hadd_pd (sum, sum);
        c = sum[0];
        // get current system time
        gettimeofday(&end, NULL);
        double elapsed = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec)/1000000.0;
        // compute and print the elapsed time in miliseconds
        //printf("Number of trials: %d\n", NUMBER_OF_TRIALS);
        //printf("Time elapsed: %f\n", elapsed);
        //printf("c = %f\n", c);
        printf("%d, %f, %f\n",NUMBER_OF_TRIALS, elapsed, c);
    }
    return 0;
}
