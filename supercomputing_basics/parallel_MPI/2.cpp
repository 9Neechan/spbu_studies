#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <iostream>

 
int main(int argc, char* argv[])
{
    int ProcRank, ProcNum;
    double calculatedDot;
    MPI_Status Status;

    MPI_Init(&argc,&argv);

    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

    if (ProcRank == 0) {
        printf("%d,\n", ProcNum);
    }

    for (int n = 1000; n <= 100000000; n *= 10) {

        int N = n;
        double *dataVector1, *dataVector2;
        dataVector1 = (double *) malloc(N * sizeof (double));
        dataVector2 = (double *) malloc(N * sizeof (double));
    
        if (ProcRank == 0) {
            for (int i = 0; i < N; ++i) {
                dataVector1[i] = rand() / (double) RAND_MAX;
                dataVector2[i] = rand() / (double) RAND_MAX;
            }
        } 

        double start, stop; 
        start = MPI_Wtime();

        MPI_Bcast(dataVector1, N, MPI_DOUBLE, 0, MPI_COMM_WORLD);
        MPI_Bcast(dataVector2, N, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    
        int k = N / ProcNum;
        int i1 = k * ProcRank;
        int i2 = k * (ProcRank + 1);
        if (ProcRank == ProcNum - 1) { 
            i2 = N;
        }
    
        double localDot = 0;
        for (int i = i1; i < i2; ++i) {
            localDot += dataVector1[i] * dataVector2[i];
        }

        MPI_Reduce(&localDot, &calculatedDot, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

        stop = MPI_Wtime();

        if (ProcRank == 0) {
            if (n == 100000000) {
                printf("%f,", stop - start);
            } else {
                printf("%f,\n", stop - start);
            }
        }

        free(dataVector1);
        free(dataVector2);
    }
    
    MPI_Finalize();
 
    return 0;
}
