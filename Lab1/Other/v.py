import matplotlib.pyplot as plt

# Given results
processes = [1, 2, 3, 4, 5, 6]
times = [0.220824, 0.029089, 0.026507, 0.023243, 0.023600, 0.017536]

# Compute speedup
t1 = times[0]
speedup = [t1 / t for t in times]

# Ideal speedup line
ideal = processes

# Plot
plt.figure(figsize=(10, 6))
plt.plot(processes, speedup, marker='o', linewidth=2, markersize=8, color='#2E86AB', label="Measured speedup")
plt.plot(processes, ideal, marker='s', linewidth=2, markersize=8, color='#A23B72', linestyle='--', label="Ideal speedup")

plt.xlabel("Number of processes", fontsize=12)
plt.ylabel("Speedup", fontsize=12)
plt.title("MPI Counting Speedup", fontsize=14, fontweight='bold')
plt.legend(fontsize=11)
plt.grid(True, alpha=0.3, linestyle='--')

plt.tight_layout()
plt.show()
