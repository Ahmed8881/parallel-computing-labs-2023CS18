import matplotlib.pyplot as plt

# Message sizes
sizes = [1, 1000, 10000]

# MPI_Bcast times (seconds)
bcast_times_2 = [0.000011, 0.000363, 0.000079]
bcast_times_4 = [0.000259, 0.000538, 0.000477]
bcast_times_8 = [0.005469, 0.000318, 0.000658]

# Linear Broadcast times (seconds)
linear_times_2 = [0.000003, 0.000004, 0.000015]
linear_times_4 = [0.000005, 0.000017, 0.000054]
linear_times_8 = [0.000205, 0.000562, 0.000148]

# Plot
plt.figure(figsize=(10,6))

plt.plot(sizes, bcast_times_2, 'o-', label='MPI_Bcast (2 processes)')
plt.plot(sizes, bcast_times_4, 's-', label='MPI_Bcast (4 processes)')
plt.plot(sizes, bcast_times_8, '^-', label='MPI_Bcast (8 processes)')

plt.plot(sizes, linear_times_2, 'o--', label='Linear Broadcast (2 processes)')
plt.plot(sizes, linear_times_4, 's--', label='Linear Broadcast (4 processes)')
plt.plot(sizes, linear_times_8, '^--', label='Linear Broadcast (8 processes)')

plt.xlabel('Message Size (ints)')
plt.ylabel('Time (seconds)')
plt.title('MPI_Bcast vs Linear Broadcast Performance')
plt.xscale('log')
plt.yscale('log')
plt.grid(True, which='both', linestyle='--', linewidth=0.5)
plt.legend()
plt.show()
