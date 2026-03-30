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
        data = 1000;
        printf("Process %d broadcasting value %d\n", rank, data);
    }

    MPI_Barrier(MPI_COMM_WORLD);
    start_time = MPI_Wtime();

    if (rank == 0) {
        for (int i = 1; i < size; i++) {
            MPI_Send(&data, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
        }
    } 
    else {
        MPI_Recv(&data, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }

    end_time = MPI_Wtime();

    printf("Process %d received data = %d\n", rank, data);

    if (rank == 0) {
        printf("Point-to-Point broadcast time with %d processes: %f seconds\n",
               size, end_time - start_time);
    }

    MPI_Finalize();
    return 0;
}
