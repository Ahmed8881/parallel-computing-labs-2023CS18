import random
import time
N = 100_000_000
TARGET = 42
arr = [random.randint(0, 99) for _ in range(N)]
start = time.time()
count = sum(1 for x in arr if x == TARGET)
end = time.time()
print(f"Count = {count}")
print(f"Time = {end - start:.2f} seconds")