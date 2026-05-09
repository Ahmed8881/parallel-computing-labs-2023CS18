import os
import pandas as pd
import matplotlib.pyplot as plt

plt.style.use('seaborn-v0_8-whitegrid')

HERE = os.path.dirname(os.path.abspath(__file__))
CSV = os.path.join(HERE, 'timings.csv')
OUT = HERE

df = pd.read_csv(CSV)

# ensure threads column is integer and sorted
df['threads'] = df['threads'].astype(int)
threads_sorted = sorted(df['threads'].unique())

approaches = ['Pthreads', 'MPI_P2P', 'MPI_Collectives', 'OpenMP_Reduction', 'OpenMP_Critical']

# Figure 1: speedup comparison
plt.figure(figsize=(8,5))
for ap in approaches:
    sub = df[df['approach'] == ap].sort_values('threads')
    if sub.empty:
        continue
    plt.plot(sub['threads'], sub['speedup'], marker='o', label=ap)

plt.plot(threads_sorted, threads_sorted, '--k', label='Ideal')
plt.xlabel('Number of Threads/Processes')
plt.ylabel('Speedup')
plt.title('Speedup Comparison: Pthreads vs MPI vs OpenMP')
plt.legend()
plt.grid(True)
plt.tight_layout()
plt.savefig(os.path.join(OUT, 'speedup_comparison.png'))
plt.close()

# Figure 2: grouped bar chart of execution time
pivot = df.pivot_table(index='threads', columns='approach', values='time_seconds')
pivot = pivot.reindex(index=threads_sorted)

fig, ax = plt.subplots(figsize=(10,5))
width = 0.12
xs = range(len(pivot.index))
for i, col in enumerate([c for c in approaches if c in pivot.columns]):
    ax.bar([x + i*width for x in xs], pivot[col].values, width=width, label=col)

ax.set_xticks([x + width*2 for x in xs])
ax.set_xticklabels([str(x) for x in pivot.index])
ax.set_xlabel('Number of Threads/Processes')
ax.set_ylabel('Time (seconds)')
ax.set_title('Execution Time Comparison')
ax.legend()
plt.tight_layout()
plt.savefig(os.path.join(OUT, 'time_comparison.png'))
plt.close()

# Figure 3: OpenMP internal comparison (Reduction vs Critical)
plt.figure(figsize=(8,5))
for ap in ['OpenMP_Reduction', 'OpenMP_Critical']:
    sub = df[df['approach'] == ap].sort_values('threads')
    if sub.empty:
        continue
    plt.plot(sub['threads'], sub['speedup'], marker='o', label=ap)

plt.plot(threads_sorted, threads_sorted, '--k', label='Ideal')
plt.xlabel('Number of Threads')
plt.ylabel('Speedup')
plt.title('OpenMP: Reduction vs Critical Section')
plt.legend()
plt.grid(True)
plt.tight_layout()
plt.savefig(os.path.join(OUT, 'openmp_internal.png'))
plt.close()

print('Plots saved.')
