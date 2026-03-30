import matplotlib.pyplot as plt

# Number of processes
processes = [1, 2, 4, 8]

# Measured execution times (seconds)
times = [0.000001, 0.000004, 0.000018, 0.000009]

# Compute speedup
t1 = times[0]
speedup = [t1 / t for t in times]

# Ideal speedup line
ideal_speedup = processes

plt.figure()
plt.plot(processes, speedup, marker='o', label='Measured speedup')
plt.plot(processes, ideal_speedup, marker='x', label='Ideal speedup')

plt.xlabel('Number of processes')
plt.ylabel('Speedup')
plt.title('Speedup vs Number of Processes')
plt.legend()
plt.grid(True)

plt.savefig("exercise1_speedup.png", dpi=300, bbox_inches='tight')
plt.show()
