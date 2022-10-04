#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>

#define ARRAY_SIZE 2048
//#define NUMBER_OF_TRIALS 2

/*
 * Statically allocate our arrays.  Compilers can
 * align them correctly.
 */
static double a[ARRAY_SIZE], b[ARRAY_SIZE], c;
struct timeval start, end;

int main(int argc, char *argv[]) {
    int i,t;

    double m = 1.0001;

    /* Populate A and B arrays */
    for (i=0; i < ARRAY_SIZE; i++) {
        b[i] = i;
        a[i] = i+1;
    }

        // get current system time using gettimeofday()


    for (int x=100000; x<=1000000; x+=100000)
    {
        int NUMBER_OF_TRIALS = x;
        gettimeofday(&start, NULL);

        /* Perform an operation a number of times */
        for (t=0; t < NUMBER_OF_TRIALS; t++) {
            for (i=0; i < ARRAY_SIZE; i++) {
                c += m*a[i] + b[i];
            }
        }

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
