#!/usr/bin/env bash
set -euo pipefail
cd /mnt/d/PDCL/Lab6/Task

# compile
gcc -fopenmp -O2 -Wall -o hello_omp hello_omp.c
gcc -fopenmp -O2 -Wall -o count_omp count_omp.c
gcc -fopenmp -O2 -Wall -o count_omp_critical count_omp_critical.c
gcc -fopenmp -O2 -Wall -o count_omp_parallel_init count_omp_parallel_init.c

OUTDIR=/mnt/d/PDCL/Lab6/Other
mkdir -p "$OUTDIR"

THREADS="1 2 4 8"

# run each program and save outputs
for t in $THREADS; do
  export OMP_NUM_THREADS=$t

  echo "=== OpenMP Reduction threads=$t ===" | tee "$OUTDIR/output_reduction_${t}.txt"
  ./count_omp 2>&1 | tee -a "$OUTDIR/output_reduction_${t}.txt"

  echo "=== OpenMP Critical threads=$t ===" | tee "$OUTDIR/output_critical_${t}.txt"
  ./count_omp_critical 2>&1 | tee -a "$OUTDIR/output_critical_${t}.txt"

  echo "=== OpenMP ParallelInit threads=$t ===" | tee "$OUTDIR/output_parallelinit_${t}.txt"
  ./count_omp_parallel_init 2>&1 | tee -a "$OUTDIR/output_parallelinit_${t}.txt"
done

# optional hello
export OMP_NUM_THREADS=4
./hello_omp > "$OUTDIR/hello_omp_output.txt" 2>&1

# parse outputs and make CSV (uses Time line for reduction/critical; uses Total time for parallel_init)
CSV="$OUTDIR/measured_lab6_timings.csv"
echo "approach,threads,time_seconds" > "$CSV"

# Reduction
for f in "$OUTDIR"/output_reduction_*.txt; do
  t=$(basename "$f" | sed -E 's/.*_([0-9]+)\.txt/\1/')
  time=$(grep -m1 "Time (seconds):" "$f" | awk '{print $3}')
  echo "OpenMP_Reduction,$t,$time" >> "$CSV"
done

# Critical
for f in "$OUTDIR"/output_critical_*.txt; do
  t=$(basename "$f" | sed -E 's/.*_([0-9]+)\.txt/\1/')
  time=$(grep -m1 "Time (seconds):" "$f" | awk '{print $3}')
  echo "OpenMP_Critical,$t,$time" >> "$CSV"
done

# Parallel init (use Total time)
for f in "$OUTDIR"/output_parallelinit_*.txt; do
  t=$(basename "$f" | sed -E 's/.*_([0-9]+)\.txt/\1/')
  time=$(grep -m1 "Total time (seconds):" "$f" | awk '{print $3}')
  # fallback if Total not present
  if [ -z "$time" ]; then
    time=$(grep -m1 "Time (seconds):" "$f" | awk '{print $3}')
  fi
  echo "OpenMP_ParallelInit,$t,$time" >> "$CSV"
done

# compute speedups relative to thread=1 for each approach and create final CSV
FINAL="$OUTDIR/measured_lab6_timings_with_speedup.csv"
echo "approach,threads,time_seconds,speedup" > "$FINAL"
for approach in OpenMP_Reduction OpenMP_Critical OpenMP_ParallelInit; do
  base=$(awk -F, -v a="$approach" '$1==a && $2==1 {print $3}' "$CSV")
  if [ -z "$base" ]; then
    echo "Warning: no baseline for $approach"
    continue
  fi
  awk -F, -v a="$approach" -v b="$base" 'BEGIN{OFS=","} $1==a {printf "%s,%s,%.6f,%.2f\n",$1,$2,$3, (b/$3)}' "$CSV" >> "$FINAL"
done

echo "Outputs saved to $OUTDIR/ (output_*.txt)"
echo "CSV files: $CSV and $FINAL"
echo "Now paste the contents of $FINAL or the raw outputs here and I'll merge into `Other/timings.csv`."