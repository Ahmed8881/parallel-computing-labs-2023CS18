# Lab 6 — Introduction to OpenMP

This lab contains a set of small OpenMP exercises: a Hello World, a parallel counting program using reduction, a critical-section variant, and a version that initializes the array in parallel before counting.

Directory structure

- Task/
  - `hello_omp.c` — OpenMP hello world
  - `count_omp.c` — parallel counting using `reduction`
  - `count_omp_critical.c` — counting with per-thread local counts merged via `#pragma omp critical`
  - `count_omp_parallel_init.c` — parallel initialization (using `rand_r`) and counting
- Other/
  - `timings.csv` — placeholder timings (replace with your measured values)
  - `plot.py` — plotting script that produces PNGs in this folder

Objectives

- Learn basic OpenMP parallel regions and worksharing (`parallel`, `for`).
- Compare reduction vs critical-section performance and understand tradeoffs.
- Use thread-private RNG state via `rand_r` for parallel initialization.

Compile instructions

All C programs are portable and compile with GCC + OpenMP on Linux/WSL. Example compile commands:

```bash
gcc -fopenmp -O2 -Wall -o hello_omp hello_omp.c
gcc -fopenmp -O2 -Wall -o count_omp count_omp.c
gcc -fopenmp -O2 -Wall -o count_omp_critical count_omp_critical.c
gcc -fopenmp -O2 -Wall -o count_omp_parallel_init count_omp_parallel_init.c
```

Run examples (setting number of threads):

```bash
export OMP_NUM_THREADS=4   # Bash / WSL
./hello_omp
./count_omp
./count_omp_critical
./count_omp_parallel_init
```

On Windows PowerShell (example):

```powershell
$env:OMP_NUM_THREADS=4
.\hello_omp.exe
```

How to run the plotting script

From the `Other/` folder run:

```bash
python plot.py
```

Three PNGs will be saved into the `Other/` folder:

- `speedup_comparison.png`
- `time_comparison.png`
- `openmp_internal.png`

Exercise descriptions

- `hello_omp.c` — demonstrates the fork-join model: all threads execute within the parallel region and then join.
- `count_omp.c` — fills an array sequentially then counts occurrences of `TARGET` in parallel using `reduction`.
- `count_omp_critical.c` — each thread accumulates a `local_count` and merges via a `#pragma omp critical` section.
- `count_omp_parallel_init.c` — initializes the array in parallel using `rand_r` with per-thread seeds, then performs parallel counting; it reports init time, count time, and total time.

Self-assessment answers

Q1 — Fork-join model vs Pthreads
- OpenMP exposes a fork-join model where the runtime creates a team of threads at the start of a `parallel` region and joins them at the end. Pthreads provides explicit thread creation and management; you must write the bookkeeping yourself. OpenMP is higher-level and easier to use for common parallel patterns.

Q2 — Purpose of the `reduction` clause (with example)
- The `reduction` clause automatically performs a thread-local accumulation and a final combine step. Example: `#pragma omp parallel for reduction(+:sum)` lets each thread keep a private `sum` and safely combine them at the end without explicit synchronization.

Q3 — Why no mutex needed in OpenMP counting (reduction)
- With `reduction` OpenMP provides a private copy per thread and performs the safe combine at the end, so no mutex/critical section is required and contention is avoided.

Q4 — `#pragma omp parallel for` vs separate `#pragma omp parallel` + `#pragma omp for`
- `parallel for` is a convenience that combines `parallel` and `for` into a single region. Using separate `parallel` and `for` gives you more control when you need persistent thread-local data across loops or multiple worksharing constructs inside the same team.

Q5 — Two ways to set thread count
- Export environment variable: `export OMP_NUM_THREADS=4` (Bash) or `$env:OMP_NUM_THREADS=4` (PowerShell).
- Use runtime call: `omp_set_num_threads(4);` before the parallel region.

Q6 — Is speedup linear? Why not? (Amdahl, memory)
- Speedup is rarely perfectly linear. Amdahl's law limits speedup based on the serial fraction. Other factors include synchronization overhead, load imbalance, thread creation overhead, and shared-memory bandwidth/latency limits.

Q7 — OpenMP vs Pthreads — complexity and performance tradeoffs
- OpenMP simplifies parallelism and reduces boilerplate; it's easier to get correct for loop-level parallelism. Pthreads can offer fine-grained control and sometimes slightly better performance in tuned cases, but it's more verbose and error-prone. For many data-parallel tasks OpenMP is the better productivity choice.

---
Replace `Other/timings.csv` placeholder values with your measured timings and re-run `plot.py` to regenerate the figures.
