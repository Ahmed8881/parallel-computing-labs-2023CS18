#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define ARRAY_SIZE 10

int main(int argc, char *argv[])
{
    int rank, size;
    int send_buffer[ARRAY_SIZE];
    int recv_buffer[ARRAY_SIZE];
    MPI_Status status;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank != 0) 
    {
        for (int i = 0; i < ARRAY_SIZE; i++)
            send_buffer[i] = rank * 100 + i;

        MPI_Send(send_buffer, ARRAY_SIZE, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }
    else 
    {
        printf("Process 0 is receiving arrays using MPI_ANY_SOURCE...\n\n");

        for (int i = 1; i < size; i++)
        {
            MPI_Recv(recv_buffer, ARRAY_SIZE, MPI_INT,
                     MPI_ANY_SOURCE, MPI_ANY_TAG,
                     MPI_COMM_WORLD, &status);

            int source = status.MPI_SOURCE;
            printf("Received array from process %d: ", source);
            for (int j = 0; j < ARRAY_SIZE; j++)
                printf("%d ", recv_buffer[j]);
            printf("\n");
        }

        printf("\nObservation: The order of messages may vary on different runs.\n");
    }

    MPI_Finalize();
    return 0;
}