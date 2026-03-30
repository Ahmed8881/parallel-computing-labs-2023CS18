import matplotlib.pyplot as plt

# MPI data
mpi_procs = [1, 2, 3, 4, 5, 6]
mpi_times = [0.220824, 0.029089, 0.026507, 0.023243, 0.023600, 0.017536]

# Pthreads data
pthread_threads = [1, 2, 4, 8]
pthread_times = [0.001443, 0.000950, 0.026915, 0.005246]



mpi_t1 = mpi_times[0]
mpi_speedup = [mpi_t1 / t for t in mpi_times]

pthread_t1 = pthread_times[0]
pthread_speedup = [pthread_t1 / t for t in pthread_times]

ideal_x = [1, 2, 3, 4, 5, 6, 7, 8]
ideal_y = [1, 2, 3, 4, 5, 6, 7, 8]



plt.figure(figsize=(8,6))

plt.plot(ideal_x, ideal_y, linestyle='--', marker='o', label='Ideal speedup')
plt.plot(mpi_procs, mpi_speedup, marker='o', label='MPI speedup')
plt.plot(pthread_threads, pthread_speedup, marker='o', label='Pthreads speedup')

plt.xlabel("Number of processes / threads")
plt.ylabel("Speedup")
plt.title("Ideal vs MPI vs Pthreads Speedup")
plt.legend()
plt.grid(True)

plt.tight_layout()
plt.show()
