import subprocess
import csv
import re

results = []

def run(cmd, label, threads):
    print("Running:", label, threads)
    out = subprocess.check_output(cmd, shell=True, text=True)

    # find time number
    m = re.search(r"Time.*?([0-9]+\.[0-9]+)", out)
    if not m:
        print(out)
        raise RuntimeError("Time not found")

    t = float(m.group(1))
    results.append([label, threads, t])

# ----------------------------
# C sequential
# ----------------------------
run("./count_seq", "C_sequential", 1)

# ----------------------------
# C pthread (recompile with different thread counts)
# ----------------------------
for th in [1,2,4,8]:
    subprocess.run(
        f"gcc -O2 -pthread -DNUM_THREADS={th} -o count_pthread count_pthread.c",
        shell=True,
        check=True
    )
    run("./count_pthread", "C_pthread", th)

# ----------------------------
# Python sequential
# ----------------------------
run("python3 count_seq.py", "Python_sequential", 1)

# ----------------------------
# Python threads
# ----------------------------
run("python3 count_threads.py", "Python_threads", 4)

# ----------------------------
# Python multiprocessing
# ----------------------------
run("python3 count_mp.py", "Python_multiprocessing", 4)


# ----------------------------
# write csv
# ----------------------------
with open("timings.csv", "w", newline="") as f:
    writer = csv.writer(f)
    writer.writerow(["program", "threads", "time_seconds"])
    writer.writerows(results)

print("\nSaved timings.csv")
