import pandas as pd
import matplotlib.pyplot as plt
import matplotlib.ticker as ticker

df = pd.read_csv('./timing_data.csv')

sizes  = [10000, 50000, 100000]
colors = ['#2196F3', '#FF5722', '#4CAF50']
procs  = [1, 2, 4]

# ── Figure 1: Speedup vs Processes (one line per problem size) ──────────────
fig, ax = plt.subplots(figsize=(7, 5))

for size, color in zip(sizes, colors):
    sub = df[df['ProblemSize'] == size].sort_values('Processes')
    ax.plot(sub['Processes'], sub['Speedup'], 'o-',
            color=color, linewidth=2, markersize=7,
            label=f'n = {size:,}')

ax.plot(procs, procs, 'k--', linewidth=1.5, label='Ideal speedup')
ax.set_xlabel('Number of Processes', fontsize=12)
ax.set_ylabel('Speedup',             fontsize=12)
ax.set_title('Speedup vs Number of Processes (Parallel BFS)', fontsize=13, fontweight='bold')
ax.set_xticks(procs)
ax.legend(fontsize=10)
ax.grid(True, linestyle='--', alpha=0.5)
plt.tight_layout()
plt.savefig('./speedup_plot.png', dpi=150)
plt.close()
print("speedup_plot.png saved")

# ── Figure 2: Time vs Problem Size (sequential vs parallel) ─────────────────
fig, ax = plt.subplots(figsize=(7, 5))

seq_times = [df[(df['ProblemSize']==s) & (df['Processes']==1)]['SequentialTime'].values[0]
             for s in sizes]
p2_times  = [df[(df['ProblemSize']==s) & (df['Processes']==2)]['ParallelTime'].values[0]
             for s in sizes]
p4_times  = [df[(df['ProblemSize']==s) & (df['Processes']==4)]['ParallelTime'].values[0]
             for s in sizes]

ax.plot(sizes, seq_times, 'rs-', linewidth=2, markersize=7, label='Sequential')
ax.plot(sizes, p2_times,  'bo-', linewidth=2, markersize=7, label='Parallel (2 procs)')
ax.plot(sizes, p4_times,  'g^-', linewidth=2, markersize=7, label='Parallel (4 procs)')

ax.set_xlabel('Problem Size (nodes)', fontsize=12)
ax.set_ylabel('Time (seconds)',       fontsize=12)
ax.set_title('Execution Time vs Problem Size (Parallel BFS)', fontsize=13, fontweight='bold')
ax.xaxis.set_major_formatter(ticker.FuncFormatter(lambda x, _: f'{int(x):,}'))
ax.legend(fontsize=10)
ax.grid(True, linestyle='--', alpha=0.5)
plt.tight_layout()
plt.savefig('./scaling_plot.png', dpi=150)
plt.close()
print("scaling_plot.png saved")

# ── Figure 3: Efficiency vs Processes ───────────────────────────────────────
fig, ax = plt.subplots(figsize=(7, 5))

for size, color in zip(sizes, colors):
    sub = df[df['ProblemSize'] == size].sort_values('Processes')
    ax.plot(sub['Processes'], sub['Efficiency'] * 100, 'o-',
            color=color, linewidth=2, markersize=7,
            label=f'n = {size:,}')

ax.axhline(100, color='k', linestyle='--', linewidth=1.5, label='Ideal (100%)')
ax.set_xlabel('Number of Processes', fontsize=12)
ax.set_ylabel('Efficiency (%)',       fontsize=12)
ax.set_title('Parallel Efficiency vs Number of Processes', fontsize=13, fontweight='bold')
ax.set_xticks(procs)
ax.legend(fontsize=10)
ax.grid(True, linestyle='--', alpha=0.5)
plt.tight_layout()
plt.savefig('./efficiency_plot.png', dpi=150)
plt.close()
print("efficiency_plot.png saved")