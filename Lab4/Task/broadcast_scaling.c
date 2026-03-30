#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int message_sizes[] = {1, 1000, 10000}; // message sizes

    for (int m = 0; m < 3; m++) {
        int n = message_sizes[m];
        int *data = (int*) malloc(n * sizeof(int));

        if (rank == 0) {
            for (int i = 0; i < n; i++) data[i] = i; // initialize
        }

        // ----- MPI_Bcast -----
        MPI_Barrier(MPI_COMM_WORLD);
        double start = MPI_Wtime();
        MPI_Bcast(data, n, MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Barrier(MPI_COMM_WORLD);
        double end = MPI_Wtime();
        if (rank == 0) printf("MPI_Bcast: size=%d, processes=%d, time=%f sec\n", n, size, end-start);

        // ----- Manual Linear Broadcast -----
        MPI_Barrier(MPI_COMM_WORLD);
        start = MPI_Wtime();
        if (rank == 0) {
            for (int p = 1; p < size; p++)
                MPI_Send(data, n, MPI_INT, p, 0, MPI_COMM_WORLD);
        } else {
            MPI_Recv(data, n, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
        MPI_Barrier(MPI_COMM_WORLD);
        end = MPI_Wtime();
        if (rank == 0) printf("Linear Broadcast: size=%d, processes=%d, time=%f sec\n\n", n, size, end-start);

        free(data);
    }

    MPI_Finalize();
    return 0;
}
