#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int x = rank + 1;       // initial value for each process
    int prefix_sum = x;     // store the accumulated sum

    int received;

    // Linear pipeline: process 0 -> 1 -> 2 -> ... -> size-1
    if(rank != 0) {
        // receive accumulated sum from previous process
        MPI_Recv(&received, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        prefix_sum += received; // add own value
    }

    if(rank != size - 1) {
        // send accumulated sum to next process
        MPI_Send(&prefix_sum, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD);
    }

    printf("Rank %d: prefix sum = %d\n", rank, prefix_sum);

    MPI_Finalize();
    return 0;
}
