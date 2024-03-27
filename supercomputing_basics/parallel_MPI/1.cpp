#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <iostream>

using namespace std;
 
int main(int argc, char* argv[])
{
    int ProcRank, ProcNum;
    double calculatedMax;
    MPI_Status Status;

    MPI_Init(&argc,&argv);

    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

    if (ProcRank == 0) {
        cout << ProcNum << ",";
    }

    for (int n = 1000; n <= 100000000; n *= 10) {

        int N = n;
        double *dataVector;
        dataVector = (double *) malloc(N * sizeof (double)); 

        if (ProcRank == 0) {
            for (int i = 0; i < N; ++i) {
                dataVector[i] = rand() / (double) RAND_MAX;
            }
        } 

        double start, stop; 
        start = MPI_Wtime();

        MPI_Bcast(dataVector, N, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    
        int k = N / ProcNum;
        int i1 = k * ProcRank;
        int i2 = k * (ProcRank + 1);
        if (ProcRank == ProcNum - 1) { 
            i2 = N;
        }
    
        double localMax = dataVector[i1];
        for (int i = i1; i < i2; ++i) {
            if (dataVector[i] > localMax) {
                localMax = dataVector[i];
            }
        }

        MPI_Reduce(&localMax, &calculatedMax, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);

        stop = MPI_Wtime();

        if (ProcRank == 0) {
            if (n == 100000000) {
                cout << stop - start;
            } else {
                cout << stop - start << endl;
            }
        }

        free(dataVector);
    }

    MPI_Finalize();
 
    return 0;
}
