import random
import time
import threading
N = 100_000_000
TARGET = 42
NUM_THREADS = 1
arr = [random.randint(0, 99) for _ in range(N)]
global_count = 0
lock = threading.Lock()
def count_chunk(start, end):
    local_count = 0
    for i in range(start, end):
        if arr[i] == TARGET:
            local_count += 1
    with lock:
        global global_count
        global_count += local_count
threads = []
chunk_size = N // NUM_THREADS
start_time = time.time()
for t in range(NUM_THREADS):
    s = t * chunk_size
    e = N if t == NUM_THREADS-1 else (t+1) * chunk_size
    thread = threading.Thread(target=count_chunk, args=(s, e))
    threads.append(thread)
    thread.start()
for t in threads:
    t.join()
end_time = time.time()
print(f"Count = {global_count}")
print(f"Time with {NUM_THREADS} threads = {end_time - start_time:.2f} seconds")
