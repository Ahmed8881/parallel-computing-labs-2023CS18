# Lab 1: Introduction to MPI (Message Passing Interface)

## Overview
Lab 1 introduces the Message Passing Interface (MPI) standard for distributed-memory parallel programming. Students learn fundamental MPI concepts including process initialization, rank identification, basic point-to-point communication with send/receive, and performance measurement. The lab includes a basic "Hello World" MPI program, a ping-pong benchmark for measuring communication latency, and a parallel counting application using MPI.

## Files
- `hello_mpi.c` – Basic MPI program demonstrating process initialization and rank identification
- `pingpong.c` – Ping-pong benchmark that measures communication latency between two MPI processes
- `count_mpi.c` – Parallel counting algorithm using MPI for distributed computation across multiple processes

## Compilation and Execution

### General MPI Compilation
MPI programs require the MPI compiler wrapper (usually `mpicc` or `mpcc`):

```bash
mpicc -o hello_mpi hello_mpi.c
mpicc -o pingpong pingpong.c
mpicc -o count_mpi count_mpi.c
```

### Running MPI Programs
Use `mpirun` or `mpiexec` to launch programs with specified number of processes:

```bash
# Hello MPI with 4 processes
mpirun -np 4 ./hello_mpi

# Ping-pong benchmark (typically 2 processes)
mpirun -np 2 ./pingpong

# Count MPI with 4 processes
mpirun -np 4 ./count_mpi
```

## Example Output

### Hello MPI (4 processes)
```
Hello from process 0 of 4
Hello from process 1 of 4
Hello from process 2 of 4
Hello from process 3 of 4
```

### Ping-Pong Benchmark
```
Ping-pong between process 0 and 1
Process 0 sent message
Process 1 received and sent back
Process 0 received response
Latency: X microseconds
```

### Count MPI
```
Process 0: Counting elements from index 0 to 25000000
Process 1: Counting elements from index 25000000 to 50000000
Process 2: Counting elements from index 50000000 to 75000000
Process 3: Counting elements from index 75000000 to 100000000
Total count = 1000042
Time = X.XX seconds
```

## Challenges Faced

### MPI Environment Setup
**Challenge**: MPI installation and configuration varies across systems (MPICH, OpenMPI, etc.).
**Resolution**: Identified the system's MPI implementation and ensured `mpicc` compiler wrapper was available in PATH.

### Understanding MPI Rank and Size
**Challenge**: Properly identifying which process is responsible for which task in a distributed system.
**Resolution**: Used `MPI_Comm_rank()` to get unique process identifier and `MPI_Comm_size()` to get total number of processes, then assigned work based on rank.

### Load Balancing in Parallel Tasks
**Challenge**: Distributing 100 million elements evenly across processes.
**Resolution**: Calculated contiguous chunks for each process: `chunk_size = N / num_processes`, with the last process handling remainders.

### Measuring Communication Overhead
**Challenge**: Isolating communication time from computation time in the ping-pong benchmark.
**Resolution**: Used `MPI_Wtime()` for high-resolution timing, placed timing code around the communication operations, and multiple iterations for more reliable measurements.

### Process Synchronization
**Challenge**: Ensuring all processes reach a checkpoint before proceeding (important for fair timing measurements).
**Resolution**: Used `MPI_Barrier()` to synchronize all processes at critical points in the program.
