import matplotlib.pyplot as plt

# Number of threads
threads = [1, 2, 4, 8]

# Execution times in seconds from your table
times = [0.07, 0.03, 0.03, 0.05]

# Calculate speedup
speedup = [times[0]/t for t in times]

# Ideal linear speedup for reference
ideal_speedup = threads

# Plotting
plt.figure(figsize=(8,5))
plt.plot(threads, speedup, marker='o', linestyle='-', color='blue', label='Observed Speedup')
plt.plot(threads, ideal_speedup, marker='x', linestyle='--', color='red', label='Ideal Linear Speedup')
plt.xticks(threads)
plt.xlabel('Number of Threads')
plt.ylabel('Speedup')
plt.title('C Pthreads: Speedup vs Number of Threads')
plt.grid(True)
plt.legend()
plt.savefig("c_pthreads_speedup.png", dpi=300)  # Save figure for LaTeX
plt.show()
