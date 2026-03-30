import random
import multiprocessing as mp
import time

N = 100_000_000
TARGET = 42
NUM_PROCESSES = 8

# Create array in main process
arr = [random.randint(0, 99) for _ in range(N)]
found_index = mp.Value('i', -1)

def search_chunk(start, end):
    for i in range(start, end):
        if found_index.value != -1:
            break
        if arr[i] == TARGET:
            with found_index.get_lock():
                if found_index.value == -1:
                    found_index.value = i
            break

if __name__ == "__main__":
    chunk_size = N // NUM_PROCESSES
    processes = []

    start_time = time.time()

    for t in range(NUM_PROCESSES):
        s = t * chunk_size
        e = N if t == NUM_PROCESSES-1 else (t+1) * chunk_size
        p = mp.Process(target=search_chunk, args=(s, e))
        processes.append(p)
        p.start()

    for p in processes:
        p.join()

    end_time = time.time()

    print(f"Found at index = {found_index.value}")
    print(f"Time with {NUM_PROCESSES} processes = {end_time - start_time:.2f} seconds")
