#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size != 4 && rank == 0) {
        printf("This program is designed for 4 processes.\n");
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    int total_sizes[] = {1000, 10000, 100000}; // total integers
    int iterations = 50; // average over multiple runs

    for (int s = 0; s < 3; s++) {
        int N = total_sizes[s];
        int chunk = N / size;
        int *data = NULL;

        if (rank == 0) data = (int*) malloc(N * sizeof(int)); // root data
        int *recvbuf = (int*) malloc(chunk * sizeof(int)); // each process chunk

        if (rank == 0) {
            for (int i = 0; i < N; i++) data[i] = i;
        }

        // ----- MPI_Scatter -----
        double total_scatter = 0.0;
        for (int iter = 0; iter < iterations; iter++) {
            MPI_Barrier(MPI_COMM_WORLD);
            double start = MPI_Wtime();
            MPI_Scatter(data, chunk, MPI_INT, recvbuf, chunk, MPI_INT, 0, MPI_COMM_WORLD);
            MPI_Barrier(MPI_COMM_WORLD);
            double end = MPI_Wtime();
            if (rank == 0) total_scatter += (end - start);
        }
        if (rank == 0) printf("MPI_Scatter: Total=%d ints, Avg time=%.6f sec\n", N, total_scatter/iterations);

        // ----- Manual Send Loop -----
        double total_manual = 0.0;
        for (int iter = 0; iter < iterations; iter++) {
            MPI_Barrier(MPI_COMM_WORLD);
            double start = MPI_Wtime();

            if (rank == 0) {
                for (int p = 1; p < size; p++)
                    MPI_Send(&data[p*chunk], chunk, MPI_INT, p, 0, MPI_COMM_WORLD);
                // root copies its own chunk
                for (int i = 0; i < chunk; i++) recvbuf[i] = data[i];
            } else {
                MPI_Recv(recvbuf, chunk, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            }

            MPI_Barrier(MPI_COMM_WORLD);
            double end = MPI_Wtime();
            if (rank == 0) total_manual += (end - start);
        }

        if (rank == 0) printf("Manual Send: Total=%d ints, Avg time=%.6f sec\n\n", N, total_manual/iterations);

        free(recvbuf);
        if (rank == 0) free(data);
    }

    MPI_Finalize();
    return 0;
}
