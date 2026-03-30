#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int iterations = 100; // average over 100 runs
    int local = rank + 1; // each process has its own integer (rank+1)

    int result_mpi, result_tree;

    double total_mpi = 0.0, total_tree = 0.0;

    for (int iter = 0; iter < iterations; iter++) {

        MPI_Barrier(MPI_COMM_WORLD);
        double start = MPI_Wtime();
        MPI_Reduce(&local, &result_mpi, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
        MPI_Barrier(MPI_COMM_WORLD);
        double end = MPI_Wtime();
        if (rank == 0) total_mpi += (end - start);

        int temp = local;
        int step = 1;
        MPI_Barrier(MPI_COMM_WORLD);
        start = MPI_Wtime();

        while (step < size) {
            if (rank % (2*step) == 0) {
                int recv;
                int partner = rank + step;
                if (partner < size) {
                    MPI_Recv(&recv, 1, MPI_INT, partner, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                    temp += recv;
                }
            } else if (rank % (2*step) == step) {
                int partner = rank - step;
                MPI_Send(&temp, 1, MPI_INT, partner, 0, MPI_COMM_WORLD);
                break; 
            }
            step *= 2;
        }

        MPI_Barrier(MPI_COMM_WORLD);
        end = MPI_Wtime();
        if (rank == 0) total_tree += (end - start);

        if (rank == 0) result_tree = temp;
    }

    if (rank == 0) {
        printf("Average time over %d iterations:\n", iterations);
        printf("MPI_Reduce: %.6f sec, Result = %d\n", total_mpi/iterations, result_mpi);
        printf("Binary-Tree Reduction: %.6f sec, Result = %d\n", total_tree/iterations, result_tree);
    }

    MPI_Finalize();
    return 0;
}
