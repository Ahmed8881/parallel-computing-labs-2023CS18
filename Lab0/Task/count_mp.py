import random
import time
import multiprocessing as mp
N = 100_000_000
TARGET = 42
NUM_PROCESSES = 2
# Create array in main process (will be copied to children)
arr = [random.randint(0, 99) for _ in range(N)]
def count_chunk(start, end):
    local_count = 0
    for i in range(start, end):
        if arr[i] == TARGET:
            local_count += 1
    return local_count
if __name__ == "__main__":
    chunk_size = N // NUM_PROCESSES
    pool = mp.Pool(NUM_PROCESSES)
    results = []
    start_time = time.time()
    for t in range(NUM_PROCESSES):
        s = t * chunk_size
        e = N if t == NUM_PROCESSES-1 else (t+1) * chunk_size
        results.append(pool.apply_async(count_chunk, (s, e)))
    pool.close()
    pool.join()
    total = sum(r.get() for r in results)
    end_time = time.time()
    print(f"Count = {total}")
    print(f"Time with {NUM_PROCESSES} processes = {end_time - start_time:.2f} seconds")