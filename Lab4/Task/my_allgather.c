#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int senddata = rank + 1;  // each process sends its rank + 1
    int recvbuf[size];        // buffer to hold all data

    // Initialize receive buffer with own data
    for (int i = 0; i < size; i++) recvbuf[i] = -1;
    recvbuf[rank] = senddata;

    int prev = (rank - 1 + size) % size; // previous process in ring
    int next = (rank + 1) % size;        // next process in ring
    int temp;

    double start = MPI_Wtime();

    // Ring allgather: size-1 steps
    for (int step = 1; step < size; step++) {
        // send our current data to next
        MPI_Send(&senddata, 1, MPI_INT, next, 0, MPI_COMM_WORLD);
        // receive data from previous
        MPI_Recv(&temp, 1, MPI_INT, prev, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        // store in receive buffer
        int index = (rank - step + size) % size;
        recvbuf[index] = temp;
        // update senddata for next step
        senddata = temp;
    }

    double end = MPI_Wtime();

    // Print results
    printf("Rank %d: my_Allgather result = [", rank);
    for (int i = 0; i < size; i++) {
        printf("%d", recvbuf[i]);
        if (i != size - 1) printf(", ");
    }
    printf("], Time = %f sec\n", end - start);

    MPI_Finalize();
    return 0;
}
