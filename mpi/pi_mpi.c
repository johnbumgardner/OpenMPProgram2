/*
 *  Prerequisties:
 *     The OpenMPI library must be installed in either a cluster of computers
 *     or a multicore machine.
 *     
 *  How to compile:
 *     mpicc -o calc_pi pi_mpi.c
 *
 *  How to execute:
 *     mpirun -np numProc ./calc_pi numPartitions
 *
 */

#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char*argv[]){

    // Argument checking
    if (argc != 2) {
        printf("Usage: mpirun -np numProc ./calc_pi numParititions\n");
        return -1;
    }

    // Number of times to divide the square
    int numPartitions = atoi(argv[1]);

    if (numPartitions > 46340) {
        printf("numPartitions is too large. Specify a number less than 46340.\n");
        return -1;
    }

    // MPI-specific variables
    int myRank, numProcs;
    double max_time_elapsed;

    // Number of midpoints inside the circle
    int circleCount       = 0;
    int local_circleCount = 0;

    // Width of the partition
    double interval = 1.0/(double)numPartitions;

    // Estimate of pi
    double pi = 0;

    // Initialize MPI environment
    MPI_Init(&argc, &argv);

    // How many processes are there?
    MPI_Comm_size(MPI_COMM_WORLD, &numProcs);

    // Which process am I?
    MPI_Comm_rank(MPI_COMM_WORLD, &myRank);

    // Barrier to start all threads at the same point
    MPI_Barrier(MPI_COMM_WORLD);

    //////////////////////////////////////////////////////
    // START: SECTION TO MODIFY 
    //////////////////////////////////////////////////////
    // Start the timer

    double startTime = MPI_Wtime();

    // Figure out which partitions this thread is computing
    int gridSize      = numPartitions*numPartitions;
    int numIterations = gridSize/numProcs;

    int firstPartition = myRank*numIterations;

    int lastPartition;  

    if (myRank == (numProcs - 1)) {
        lastPartition = gridSize - 1;
    } else {
        lastPartition = (myRank + 1)*numIterations - 1;
    }

    // Process the partitions
    int i;

    for (i = firstPartition; i <= lastPartition; i++) {

        int row = i/numPartitions;
        int col = i % numPartitions;

        double a = (row + .5)*interval;
        double b = (col + .5)*interval;

        if ((a*a + b*b) <= 1) {
            local_circleCount++;
        }

    }

    // Stop the timer and calculate how much time has passed
    
    double endTime = MPI_Wtime();
    double elapsedTime = endTime - startTime;

    MPI_Barrier(MPI_COMM_WORLD);
 
    // Reduce to find the maximum time elapsed

    // Reduce to calculate the number of midpoints inside the circle

    MPI_Reduce(&local_circleCount, &circleCount, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    //////////////////////////////////////////////////////
    // END: SECTION TO MODIFY 
    //////////////////////////////////////////////////////
    
    // Thread 0 will calculate pi
    if (myRank == 0) {
        pi = (double)(4*circleCount)/(numPartitions * numPartitions);
        printf("Estimate of pi is: %10.8lf\n", pi);
        printf("Total elapsed time: %10.8lf\n");
    }

    // Clean up
    MPI_Finalize();

    return 0;

}
