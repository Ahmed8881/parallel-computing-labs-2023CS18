import pandas as pd
import matplotlib.pyplot as plt

df = pd.read_csv("timings.csv")

# -----------------------------
# plot C pthread speedup
# -----------------------------

base = df[(df.program=="C_pthread") & (df.threads==1)].time_seconds.values[0]

c = df[df.program=="C_pthread"].copy()
c["speedup"] = base / c["time_seconds"]

plt.figure()
plt.plot(c["threads"], c["speedup"], marker="o", label="Measured")

plt.plot(c["threads"], c["threads"], linestyle="--", label="Ideal")

plt.xlabel("Threads")
plt.ylabel("Speedup")
plt.title("C pthread speedup")
plt.legend()
plt.grid(True)

plt.savefig("c_pthread_speedup.png")
plt.show()


# -----------------------------
# comparison bar chart
# -----------------------------

final = df.groupby("program")["time_seconds"].mean()

plt.figure()
final.plot(kind="bar")

plt.ylabel("Time (seconds)")
plt.title("Sequential vs Parallel comparison")
plt.grid(True)

plt.tight_layout()
plt.savefig("comparison.png")
plt.show()
