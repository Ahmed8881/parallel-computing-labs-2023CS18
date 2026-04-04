# Parallel and Distributed Computing Lab Portfolio

## Student Information
- Name: M Ahmed Butt
- Roll Number: 2023-CS-18
- Course: Parallel and Distributed Computing
- Instructor: Waqas Ali

## Repository Overview
This repository contains six labs (Lab 0 to Lab 5) covering the core progression of parallel and distributed programming:
- Shared-memory parallelism with pthreads and Python threading/multiprocessing
- MPI fundamentals and process communication
- Point-to-point and collective MPI patterns
- MPI performance analysis and optimization
- Graph processing case study (sequential vs MPI-based parallel BFS)

## Lab-Wise Summary

| Lab | Folder | Main Topic | Key Concepts / Implementations |
|-----|--------|------------|--------------------------------|
| 0 | `Lab0` | Parallel Programming Fundamentals | C pthreads, Python threading, Python multiprocessing, mutex synchronization, speedup analysis, GIL impact |
| 1 | `Lab1` | MPI Introduction | `hello_mpi`, `pingpong`, `count_mpi`, ranks, communicators, barriers, basic benchmarking |
| 2 | `Lab2` | MPI Point-to-Point Communication | `MPI_Send`, `MPI_Recv`, custom gather, wildcard receive, parallel sum, tags and status |
| 3 | `Lab3` | MPI Collective Communication | Broadcast, reduce, scatter/gather, scatterv/gatherv, matrix-vector multiplication |
| 4 | `Lab4` | MPI Performance and Optimization | Broadcast scaling, reduction trees, custom allgather, scan/prefix sum, overhead analysis |
| 5 | `Lab5` | Parallel BFS (MPI) Case Study | Sequential BFS vs MPI parallel BFS, speedup/efficiency measurement, plotting and report |

## Folder Structure

- `Lab0/`
	- `Task/`: core lab programs (`count_seq.c`, `count_pthread.c`, Python versions)
	- `Other/`: experiments, search examples, plotting scripts, timing data
- `Lab1/`
	- `Task/`: `hello_mpi.c`, `pingpong.c`, `count_mpi.c`
	- `Other/`: comparison scripts and generated binaries
- `Lab2/`
	- `Task/`: `send_recv.c`, `parallel_sum.c`, `custom_gather.c`, `wildcard.c`
	- `Other/`: additional experiments and helper scripts
- `Lab3/`
	- `Task/`: collective communication implementations and distributed `matvec.c`
	- `Other/`: alternate implementations and references
- `Lab4/`
	- `Task/`: optimization-oriented MPI programs (`broadcast_scaling.c`, `reduction_tree.c`, etc.)
	- `Other/`: extra benchmarks and report materials
- `Lab5/`
	- `sequential/`: sequential BFS (`sequential.c`)
	- `parallel/`: MPI parallel BFS (`parallel.c`)
	- `results/`: measured timings (`timing_data.csv`) and plotting script
	- `images/`: generated result figures
	- `report/`: lab report (`report.tex`)

## Lab 5 Performance Data (Measured)

Data source: `Lab5/results/timing_data.csv`

| Problem Size | Processes | Parallel Time (s) | Sequential Time (s) | Speedup | Efficiency |
|--------------|-----------|-------------------|----------------------|---------|------------|
| 10,000 | 1 | 0.000985 | 0.001154 | 1.17 | 1.17 |
| 10,000 | 2 | 0.001742 | 0.001154 | 0.66 | 0.33 |
| 10,000 | 4 | 0.051426 | 0.001154 | 0.02 | 0.01 |
| 50,000 | 1 | 0.010243 | 0.003442 | 0.34 | 0.34 |
| 50,000 | 2 | 0.005703 | 0.003442 | 0.60 | 0.30 |
| 50,000 | 4 | 0.020076 | 0.003442 | 0.17 | 0.04 |
| 100,000 | 1 | 0.013404 | 0.013302 | 0.99 | 0.99 |
| 100,000 | 2 | 0.016397 | 0.013302 | 0.81 | 0.41 |
| 100,000 | 4 | 0.023294 | 0.013302 | 0.57 | 0.14 |

Observation: For the tested graph generation/setup, MPI overhead dominates at higher process counts, especially for smaller problem sizes, so speedup remains sub-linear and often below 1 for 2 and 4 processes.

## How to Use This Repository

1. Open the required lab folder (`Lab0` to `Lab5`).
2. Read that lab's `README.md` for compilation and execution commands.
3. Compile with `gcc` for pthread/sequential C tasks and `mpicc` for MPI tasks.
4. Run MPI programs using `mpirun -np <processes> ./program_name`.
5. For Lab 5 analysis, use data in `Lab5/results/timing_data.csv` and figures in `Lab5/images/`.

## Tools and Technologies Used

- C (GCC)
- Python
- POSIX Threads (pthreads)
- MPI (`mpicc`, `mpirun`)
- Matplotlib/plotting scripts for analysis

## Notes

- Results can vary with machine architecture, number of CPU cores, MPI implementation, and process/thread binding.
- Some labs include both `Task/` (core submissions) and `Other/` (extra experiments/comparisons).
