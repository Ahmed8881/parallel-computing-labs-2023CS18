# Lab 4: MPI Performance Analysis and Optimization

## Overview
Lab 4 focuses on analyzing and optimizing the performance of MPI programs. Students explore various MPI communication patterns at scale, measuring overhead, throughput, and latency. The lab includes implementations of broadcast algorithms (flat vs tree-based), collective operations analysis, all-gather patterns, reduction tree structures, scattered vs contiguous memory usage, and mysterious performance anomalies. Emphasis is placed on understanding performance bottlenecks and applying optimization techniques.

## Files
- `broadcast_scaling.c` – Analyzes performance of broadcast using different algorithms (flat, tree-based)
- `collective.c` – Benchmark and comparison of MPI collective operations at scale
- `linear_scan.c` – Linear scan/prefix sum distributed computation
- `reduction_tree.c` – Tree-based reduction for better scaling than linear reduction
- `scatter_overhead.c` – Measures communication overhead of scatter operations
- `my_allgather.c` – Custom implementation of all-gather with performance analysis
- `mixed.c` – Mixed communication and computation patterns
- `distributed_prefix_sum.c` – Distributed prefix sum (scan) operation
- `tree_scan.c` – Tree-based distributed scan for better performance
- `mysterious.c` – Performance anomaly investigation and analysis

## Compilation and Execution

```bash
# Compile all programs
mpicc -O2 -o broadcast_scaling broadcast_scaling.c
mpicc -O2 -o collective collective.c
mpicc -O2 -o linear_scan linear_scan.c
mpicc -O2 -o reduction_tree reduction_tree.c
mpicc -O2 -o scatter_overhead scatter_overhead.c
mpicc -O2 -o my_allgather my_allgather.c
mpicc -O2 -o mixed mixed.c
mpicc -O2 -o distributed_prefix_sum distributed_prefix_sum.c
mpicc -O2 -o tree_scan tree_scan.c
mpicc -O2 -o mysterious mysterious.c
```

### Running the Programs

```bash
# Broadcast scaling with different process counts
mpirun -np 2 ./broadcast_scaling
mpirun -np 4 ./broadcast_scaling
mpirun -np 8 ./broadcast_scaling

# Collective operations benchmark
mpirun -np 4 ./collective

# Linear scan operation
mpirun -np 4 ./linear_scan

# Tree-based reduction
mpirun -np 4 ./reduction_tree

# Scatter overhead analysis
mpirun -np 4 ./scatter_overhead

# All-gather custom implementation
mpirun -np 4 ./my_allgather

# Mixed operations
mpirun -np 4 ./mixed

# Distributed prefix sum
mpirun -np 4 ./distributed_prefix_sum

# Tree scan
mpirun -np 4 ./tree_scan

# Mysterious performance analysis
mpirun -np 4 ./mysterious
```

## Example Output

### Broadcast Scaling
```
Broadcast scaling analysis:
Processes: 2, Message Size: 1 KB, Time: 0.000234 sec
Processes: 2, Message Size: 10 KB, Time: 0.000456 sec
Processes: 4, Message Size: 1 KB, Time: 0.000567 sec
Processes: 4, Message Size: 10 KB, Time: 0.001123 sec
Flat broadcast time: 0.005 sec
Tree broadcast time: 0.003 sec (faster for larger process counts)
```

### Collective Operations Benchmark
```
MPI_Bcast: 1000 iterations, time: 0.234 sec
MPI_Reduce: 1000 iterations, time: 0.345 sec
MPI_Scatter: 1000 iterations, time: 0.456 sec
MPI_Gather: 1000 iterations, time: 0.512 sec
MPI_Allgather: 1000 iterations, time: 0.678 sec
```

### Reduction Tree vs Linear
```
Linear reduction (2^20 int values, 8 processes):
  Time: 0.062 seconds
  
Tree-based reduction (same data):
  Time: 0.045 seconds (28% faster)
```

### Scatter Overhead
```
Message size: 1 KB, scatter time: 0.0001 sec
Message size: 10 KB, scatter time: 0.0003 sec
Message size: 100 KB, scatter time: 0.0021 sec
Message size: 1 MB, scatter time: 0.0234 sec
```

### Distributed Prefix Sum
```
Input: [1, 2, 3, 4, 5, 6, 7, 8]
Output: [1, 3, 6, 10, 15, 21, 28, 36]
Linear algorithm time: 0.045 sec
Tree algorithm time: 0.015 sec
```

## Challenges Faced

### Measuring Communication Overhead
**Challenge**: Isolating communication time from computation requires careful synchronization and timing placement.
**Resolution**: Used `MPI_Barrier()` before timing, placed `MPI_Wtime()` calls immediately around communication, and iterated multiple times to reduce measurement noise.

### Broadcast Algorithm Selection
**Challenge**: Determining whether flat or tree-based broadcast is optimal depends on system architecture and message size.
**Resolution**: Implemented both algorithms and benchmarked to show trade-offs; provided selection heuristic based on process count and message size.

### Load Balancing in Reductions
**Challenge**: Linear reduction chains have poor scaling; tree-based reduction is more efficient but more complex to implement.
**Resolution**: Implemented both approaches, measured performance differences, and demonstrated logarithmic scaling of tree reduction.

### Memory Scalability
**Challenge**: Storing all gather results requires O(n*p) memory, which becomes problematic with many processes.
**Resolution**: Used alternative algorithms like MPI_Allgather for better scalability; discussed memory-communication trade-offs.

### All-Gather Implementation Variations
**Challenge**: Multiple ways to implement all-gather (ring, tree, recursive doubling) with different characteristics.
**Resolution**: Analyzed different implementations and their performance characteristics relative to MPI's optimized MPI_Allgather.

### Performance Anomalies and Non-Linearity
**Challenge**: Unexpected performance behavior that doesn't match theoretical predictions (e.g., mysterious.c).
**Resolution**: Investigated causes: cache effects, network contention, message size thresholds for protocol changes (eager vs rendezvous).

### Profiling and Analysis Tools
**Challenge**: Understanding where time is spent (computation vs communication) in complex programs.
**Resolution**: Used manual timing of code sections, PAPI (Performance API) counters, or vendor profiling tools to identify bottlenecks.

### Scaling Efficiency Calculation
**Challenge**: Properly calculating and interpreting speedup, efficiency, and weak vs strong scaling metrics.
**Resolution**: Defined clear metrics:
- **Speedup**: $S(p) = \frac{T(1)}{T(p)}$
- **Efficiency**: $E(p) = \frac{S(p)}{p}$
- **Strong scaling**: Fixed problem size, vary processor count
- **Weak scaling**: Scale problem size with processor count
