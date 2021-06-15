/*
 *  How to compile:
 *     gcc -o calc_pi pi_openmp.c
 *
 *  How to execute:
 *     setenv OMP_NUM_THREADS numProc
 *     ./calc_pi numPartitions
 *
 */

#include <omp.h>
#include <stdio.h>

int main(int argc, char*argv[]){

    // Argument checking
    if (argc != 2) {
        printf("Usage: ./pi_mpi numParititions\n");
        return -1;
    }

    // Number of times to divide the square
    int numPartitions = atoi(argv[1]);

    if (numPartitions > 46340) {
        printf("numPartitions is too large. Specify a number less than 46340.\n");
        return -1;
    }

    int circleCount = 0;
    double interval = 0, pi = 0;
    int i = 0, j = 0;

    interval = 1.0/(double)numPartitions;
    
    // Parallelize iterations of the for-i loop

    #pragma omp parallel for default(shared) private(i) 
    {
        for (i = 0; i < numPartitions; i++) 
        {
            double a = (i + .5)*interval;
            for (j = 0; j < numPartitions; j++) 
            {
                double b = (j + .5)*interval;
                if ((a*a + b*b) <= 1) 
                {
                    circleCount++;
                }
            }
        }
    }
    
    
    
    pi = (double)(4*circleCount)/(numPartitions * numPartitions);
    printf("For-i parallelization of estimate of pi is: %10.8lf\n", pi);

    return 0;

}
