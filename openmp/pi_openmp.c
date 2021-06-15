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

    // Parallelize iterations of the for-i loop

    int circleCount = 0;
    double interval = 0, pi = 0;
    int i = 0, j = 0;

    interval = 1.0/(double)numPartitions;

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

    // Parallelize iterations of the for-j loop, one iteration of the for i loop at a time

    circleCount = 0;
    interval = 0, pi = 0;
    i = 0, j = 0;

    interval = 1.0/(double)numPartitions;

    for (i = 0; i < numPartitions; i++) 
    {
        double a = (i + .5)*interval;
        #pragma omp parallel for default(shared) private(j) 
        {
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
    printf("For-j with one i iteration done at a time parallelization of estimate of pi is: %10.8lf\n", pi);

    // Parallelize iterations of the for-j loop, without respect to whether they are in iteration of i

    circleCount = 0;
    interval = 0, pi = 0;
    i = 0, j = 0;



    interval = 1.0/(double)numPartitions;

    // write parallel to array that stores a and b
    
    double a[numPartitions];
    double b[numPartitions];

    int k = 0;
    #pragma omp parallel for default(shared) private(k) 
    {
        for (k = 0; k < numPartitions; k++) 
        {
            a[k] = (k + .5) * interval;
            b[k] = (k + .5) * interval;
        }    
    } 

    #pragma omp parallel for default(shared) private(i) 
    {
        for (i = 0; i < numPartitions; i++) 
        {
            #pragma omp parallel for default(shared) private(j) 
            {
                for (j = 0; j < numPartitions; j++) 
                {
                    if ((a[i]*a[i] + b[j]*b[j]) <= 1) 
                    {
                        circleCount++;
                    }
                }
            }
        }
    }


    
    
    
    
    pi = (double)(4*circleCount)/(numPartitions * numPartitions);
    printf("For-j without regard for i iteration done at a time parallelization of estimate of pi is: %10.8lf\n", pi);

    return 0;

}
