#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#define N 1000 // total array size
int main(int argc, char** argv) {
int rank, size;
int *array = NULL;
int *local_array;
int *sendcounts; // number of elements to send to each
int *displs; // displacements in the global array
int local_n;
int local_sum = 0, total_sum = 0;
MPI_Init(&argc, &argv);
MPI_Comm_rank(MPI_COMM_WORLD, &rank);
MPI_Comm_size(MPI_COMM_WORLD, &size);
// Compute sendcounts and displacements on root
if (rank == 0) {
array = (int*) malloc(N * sizeof(int));
for (int i = 0; i < N; i++)
array[i] = i + 1;
sendcounts = (int*) malloc(size * sizeof(int));
displs = (int*) malloc(size * sizeof(int));
int sum = 0;
for (int i = 0; i < size; i++) {
sendcounts[i] = N / size;
if (i < N % size) sendcounts[i]++; // first remainder
displs[i] = sum;
sum += sendcounts[i];
}
}

MPI_Scatter(sendcounts, 1, MPI_INT, &local_n, 1, MPI_INT, 0,MPI_COMM_WORLD);
local_array = (int*) malloc(local_n * sizeof(int));
MPI_Scatterv(array, sendcounts, displs, MPI_INT,

local_array, local_n, MPI_INT, 0, MPI_COMM_WORLD);

for (int i = 0; i < local_n; i++)
local_sum += local_array[i];
printf("Process %d: local sum = %d\n", rank, local_sum);
// Reduce to get total sum
MPI_Reduce(&local_sum, &total_sum, 1, MPI_INT, MPI_SUM, 0,MPI_COMM_WORLD);
if (rank == 0) {
printf("Total sum = %d\n", total_sum);
printf("Expected total sum = %d\n", N * (N + 1) / 2);
free(array);
free(sendcounts);
free(displs);
}
free(local_array);
MPI_Finalize();
return 0;
}