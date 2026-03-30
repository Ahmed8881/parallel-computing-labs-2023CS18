import matplotlib.pyplot as plt
import numpy as np

# Exercises
exercises = ['Exercise 1: Broadcast', 
             'Exercise 2: Reduce', 
             'Exercise 3: Scatter/Gather', 
             'Exercise 4: Scatterv/Gatherv']

# Hypothetical execution times in seconds (from your report)
# Collective (MPI_Bcast, MPI_Reduce, etc.)
collective_times = [0.000004, 0.000262, 0.000322, 0.000350]  # replace with real measurements
# Point-to-Point (MPI_Send/MPI_Recv manual)
point_times = [0.000005, 0.000322, 0.000400, 0.000480]  # replace with real measurements

x = np.arange(len(exercises))
width = 0.35  # width of bars

fig, ax = plt.subplots(figsize=(10,6))

# Bars for Collective and Point-to-Point
rects1 = ax.bar(x - width/2, collective_times, width, label='Collective', color='skyblue')
rects2 = ax.bar(x + width/2, point_times, width, label='Point-to-Point', color='salmon')

# Labels, titles, and axes
ax.set_ylabel('Execution Time (seconds)')
ax.set_title('Comparison: Point-to-Point vs Collective Communication')
ax.set_xticks(x)
ax.set_xticklabels(exercises, rotation=25, ha='right')
ax.legend()

# Add value labels on top of bars
def autolabel(rects):
    for rect in rects:
        height = rect.get_height()
        ax.annotate(f'{height:.6f}',
                    xy=(rect.get_x() + rect.get_width()/2, height),
                    xytext=(0,3),  # 3 points vertical offset
                    textcoords="offset points",
                    ha='center', va='bottom', fontsize=8)

autolabel(rects1)
autolabel(rects2)

plt.tight_layout()
plt.show()
