import matplotlib.pyplot as plt

# -------------------------
# MPI execution times (in seconds) from Task 3
processes = [1, 2, 3, 4, 5, 6]            # Number of MPI processes
times = [0.075409, 0.114599, 0.033245, 0.046969, 0.028980, 0.021793]  # Measured execution times

# -------------------------
# Calculate speedup
speedup = [times[0] / t for t in times]  # S(N) = T(1)/T(N)

# -------------------------
# Ideal speedup (linear scaling)
ideal_speedup = processes  # Ideal: S(N) = N

# -------------------------
# Plotting
plt.figure(figsize=(8,5))
plt.plot(processes, speedup, marker='o', color='blue', label='Measured Speedup')
plt.plot(processes, ideal_speedup, linestyle='--', color='red', label='Ideal Speedup')
plt.xticks(processes)
plt.xlabel("Number of MPI Processes")
plt.ylabel("Speedup")
plt.title("Speedup vs Number of MPI Processes (Task 3)")
plt.grid(True, linestyle='--', alpha=0.7)
plt.legend()

# -------------------------
# Annotate speedup values on graph
for x, y in zip(processes, speedup):
    plt.text(x, y + 0.1, f"{y:.2f}", ha='center')

# -------------------------
# Save the graph as an image
graph_file = "task3_speedup_graph.png"
plt.savefig(graph_file, dpi=300)
plt.show()

print(f"Graph saved as {graph_file}")
