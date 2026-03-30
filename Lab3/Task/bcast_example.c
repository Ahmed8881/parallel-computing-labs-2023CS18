#include <stdio.h>
#include <mpi.h>

int main(int argc, char** argv) {

    int rank, size;
    int data;
    double start_time, end_time;

    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0) {
        // Root initializes the data
        data = 1000;
        printf("Process %d broadcasting value %d\n", rank, data);
    }

    MPI_Barrier(MPI_COMM_WORLD);

    start_time = MPI_Wtime();

    MPI_Bcast(&data, 1, MPI_INT, 0, MPI_COMM_WORLD);

    end_time = MPI_Wtime();

    printf("Process %d received data = %d\n", rank, data);

    if (rank == 0) {
        printf("Broadcast time with %d processes: %f seconds\n", size, end_time - start_time);
    }

    MPI_Finalize();
    return 0;
}
