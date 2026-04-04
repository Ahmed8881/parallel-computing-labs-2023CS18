# Lab 0: Parallel Programming Fundamentals with Pthreads

## Overview
Lab 0 introduces the fundamentals of parallel programming by comparing sequential and parallel implementations of a counting task. The lab explores multi-threading using POSIX threads (pthreads) in both C and Python, demonstrating concepts like thread creation, synchronization with mutexes, and performance measurement. Students analyze speedup and performance gains from parallelization.

## Files
- `count_seq.c` – Sequential C implementation that counts occurrences of a target value in an array
- `count_seq.py` – Sequential Python implementation for comparison
- `count_pthread.c` – Parallel C implementation using pthreads with mutex synchronization
- `count_threads.py` – Parallel Python implementation using threading
- `count_mp.py` – Multiprocessing implementation in Python (alternative parallel approach)

## Compilation and Execution

### Sequential C Program
```bash
gcc -o count_seq count_seq.c -lm
./count_seq
```

### Parallel C Program (Pthread)
```bash
gcc -o count_pthread count_pthread.c -pthread -lm
# Run with single thread
./count_pthread

# Run with multiple threads (compile with thread count specified)
gcc -D NUM_THREADS=2 -o count_pthread count_pthread.c -pthread -lm
gcc -D NUM_THREADS=4 -o count_pthread count_pthread.c -pthread -lm
gcc -D NUM_THREADS=8 -o count_pthread count_pthread.c -pthread -lm
./count_pthread
```

### Python Implementations
```bash
# Sequential Python
python count_seq.py

# Threading (modify NUM_THREADS in code)
python count_threads.py

# Multiprocessing
python count_mp.py
```

## Example Output

### Sequential C (count_seq)
```
Count = 1000042
Time = 2.45 seconds
```



### Parallel C (count_pthread with 2 threads)
```
Count = 1000042
Time with 2 threads = 1.86 seconds
```

### Parallel C (count_pthread with 4 threads)
```
Count = 1000042
Time with 4 threads = 1.45 seconds
```

### Parallel C (count_pthread with 8 threads)
```
Count = 1000042
Time with 8 threads = 1.39 seconds
```

## Challenges Faced

### Thread Synchronization
**Challenge**: Ensuring correct results when multiple threads access shared data.
**Resolution**: Used POSIX mutex locks (pthread_mutex_t) to ensure atomic updates to the global count. Each thread accumulates a local count to minimize lock contention.

### Diminishing Returns with More Threads
**Challenge**: Adding more threads beyond the system's core count does not improve performance linearly (observed slowdown with 3+ threads on a 2-core system).
**Resolution**: Analyzed Amdahl's law and recognized that synchronization overhead dominates when exceeding physical cores. Optimal thread count should match the number of available CPU cores.

### Python's Global Interpreter Lock (GIL)
**Challenge**: Python threading shows poor speedup due to the Global Interpreter Lock, preventing true parallel execution.
**Resolution**: Explored multiprocessing as an alternative, which creates separate processes and bypasses the GIL limitation.

### Accurate Performance Measurement
**Challenge**: Initial use of `clock()` in C gave inaccurate results for parallel programs.
**Resolution**: Switched to `clock_gettime(CLOCK_MONOTONIC)` for more accurate wall-clock time measurement of parallel execution.

### Load Balancing
**Challenge**: Uneven data distribution across threads could lead to idle threads waiting for others.
**Resolution**: Calculated chunk boundaries to evenly distribute work: `chunk_size = N / NUM_THREADS`, with the last thread handling any remainder elements.
