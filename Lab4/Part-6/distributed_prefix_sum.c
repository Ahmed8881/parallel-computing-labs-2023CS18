#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int N = 8;                // total array size
    int local_n = N / size;   // elements per process

    int local[local_n];       // local array
    int local_prefix[local_n]; // local prefix sums

    // Initialize local array (example)
    for(int i = 0; i < local_n; i++)
        local[i] = rank*local_n + i + 1; // 1..N

    // 1. Compute local prefix sum
    local_prefix[0] = local[0];
    for(int i = 1; i < local_n; i++)
        local_prefix[i] = local_prefix[i-1] + local[i];

    // 2. Compute local total
    int local_total = local_prefix[local_n-1];

    // 3. Gather all local totals to compute offsets
    int all_totals[size];
    MPI_Allgather(&local_total, 1, MPI_INT, all_totals, 1, MPI_INT, MPI_COMM_WORLD);

    int offset = 0;
    for(int i = 0; i < rank; i++)
        offset += all_totals[i];

    // 4. Adjust local prefix sums by offset
    for(int i = 0; i < local_n; i++)
        local_prefix[i] += offset;

    // Print for verification
    printf("Process %d: ", rank);
    for(int i = 0; i < local_n; i++)
        printf("%d ", local_prefix[i]);
    printf("\n");

    MPI_Finalize();
    return 0;
}
