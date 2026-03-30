# Lab 3: Collective Communication in MPI

## Overview
Lab 3 explores collective communication operations in MPI, where all processes in a communicator participate in coordinated data exchange. Students implement and analyze broadcast, reduction, scatter/gather, and point-to-point communication patterns. The lab includes applications like broadcasting values to all processes, computing global reductions, and performing distributed matrix-vector multiplication. Collective operations provide better performance and ease of programming compared to manual point-to-point implementations.

## Files
- `bcast_example.c` – Demonstrates `MPI_Bcast` operation: broadcasting data from one process to all others
- `reduce_sum.c` – Implements global reduction to compute sum across all processes using `MPI_Reduce`
- `scatter_gather.c` – Shows `MPI_Scatter` (distribute data) and `MPI_Gather` (collect data) operations
- `scatterv_gatherv.c` – Variable-count scatter and gather for non-uniform data distribution
- `matvec.c` – Distributed matrix-vector multiplication using scatter/gather operations
- `p_to_p.c` – Point-to-point communication patterns and examples
- `code.c` – Additional collective communication examples and patterns

## Compilation and Execution

```bash
# Compile all programs
mpicc -o bcast_example bcast_example.c
mpicc -o reduce_sum reduce_sum.c
mpicc -o scatter_gather scatter_gather.c
mpicc -o scatterv_gatherv scatterv_gatherv.c
mpicc -o matvec matvec.c
mpicc -o p_to_p p_to_p.c
mpicc -o code code.c
```

### Running the Programs

```bash
# Broadcast example (send from root to all processes)
mpirun -np 4 ./bcast_example

# Reduction sum (each process contributes to global sum)
mpirun -np 4 ./reduce_sum

# Scatter/Gather operations
mpirun -np 4 ./scatter_gather

# Variable-count operations
mpirun -np 4 ./scatterv_gatherv

# Matrix-vector multiplication
mpirun -np 4 ./matvec

# Point-to-point patterns
mpirun -np 4 ./p_to_p
```

## Example Output

### Broadcast Example
```
Process 0 broadcasting value 1000
Process 0 received data = 1000
Process 1 received data = 1000
Process 2 received data = 1000
Process 3 received data = 1000
```

### Reduction Sum
```
Process 0: Local sum = 25000000
Process 1: Local sum = 75000000
Process 2: Local sum = 125000000
Process 3: Local sum = 175000000
Global sum = 400000000 (Process 0)
```

### Scatter/Gather
```
Process 0 sent: [1, 2, 3, 4, 5, 6, 7, 8]
Process 0 received: [1, 2]
Process 1 received: [3, 4]
Process 2 received: [5, 6]
Process 3 received: [7, 8]
Process 0 gathered: [1, 2, 3, 4, 5, 6, 7, 8]
```

### Variable-Count Scatter/Gather
```
Process 0 scattered 10 elements with variable counts
Process 0 received 4 elements
Process 1 received 3 elements
Process 2 received 2 elements
Process 3 received 1 element
Global gather: [all elements reassembled]
```

### Matrix-Vector Multiplication
```
Matrix shape: 16x16
Vector size: 16
Process 0 processing rows 0-3
Process 1 processing rows 4-7
Process 2 processing rows 8-11
Process 3 processing rows 12-15
Result vector computed in parallel
Time = X.XX seconds
```

## Challenges Faced

### Understanding Collective Communication Semantics
**Challenge**: Collective operations require ALL processes in a communicator to participate; missing a call causes deadlock.
**Resolution**: Ensured every process reaches every collective call in the same order, using proper control flow and barriers when necessary.

### Data Distribution Imbalance
**Challenge**: Using `MPI_Scatter` with non-uniform data sizes across processes leads to load imbalance.
**Resolution**: Used `MPI_Scatterv` for variable-count distribution, carefully calculating displacement and count arrays.

### Reduction Operations and Data Types
**Challenge**: Choosing the correct MPI reduction operation (`MPI_SUM`, `MPI_MAX`, `MPI_PROD`, etc.) for different problems.
**Resolution**: Matched the mathematical operation needed (sum, max, product) with corresponding MPI reduction operations.

### Matrix-Vector Multiplication Scaling
**Challenge**: Distributing matrix rows across processes while ensuring each process has the complete vector for computation.
**Resolution**: Broadcasted the entire vector to all processes, then scattered row chunks, allowing independent computation.

### Combining Collective and Point-to-Point Operations
**Challenge**: Mixing collective and point-to-point calls can be confusing and error-prone.
**Resolution**: Separated logical phases of computation: first collective operations (broadcast/scatter), then local computation, then collective gather/reduce.

### Performance of Collective vs Manual Implementation
**Challenge**: Understanding when to use built-in collective operations vs implementing custom patterns.
**Resolution**: Measured performance to show that MPI collective operations are highly optimized and generally outperform manual point-to-point implementations.

### Communicator Creation and Subgroups
**Challenge**: Creating subcommunicators for specific subsets of processes requires careful rank mapping.
**Resolution**: Used `MPI_Comm_split()` to partition processes into groups when needed for hierarchical algorithms.
