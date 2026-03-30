#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define N 100000000
#define TARGET 42
#ifndef NUM_THREADS
#define NUM_THREADS 8
#endif

int *arr;
int found_index = -1;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

typedef struct {
    long start;
    long end;
} ThreadArgs;

void* search_chunk(void* arg) {
    ThreadArgs *args = (ThreadArgs*) arg;
    for (long i = args->start; i < args->end; i++) {
        // Stop if found
        pthread_mutex_lock(&lock);
        int stop = (found_index != -1);
        pthread_mutex_unlock(&lock);
        if (stop) break;

        if (arr[i] == TARGET) {
            pthread_mutex_lock(&lock);
            if (found_index == -1) found_index = i; // first occurrence
            pthread_mutex_unlock(&lock);
            break;
        }
    }
    return NULL;
}

int main() {
    // Allocate and fill array
    arr = malloc(N * sizeof(int));
    if (!arr) { perror("malloc"); return 1; }
    srand(time(NULL));
    for (long i = 0; i < N; i++) arr[i] = rand() % 100;

    pthread_t threads[NUM_THREADS];
    ThreadArgs args[NUM_THREADS];
    long chunk_size = N / NUM_THREADS;

    // Start timing
    clock_t start = clock();

    // Create threads
    for (int t = 0; t < NUM_THREADS; t++) {
        args[t].start = t * chunk_size;
        args[t].end = (t == NUM_THREADS-1) ? N : (t+1) * chunk_size;
        pthread_create(&threads[t], NULL, search_chunk, &args[t]);
    }

    // Join threads
    for (int t = 0; t < NUM_THREADS; t++) pthread_join(threads[t], NULL);

    clock_t end = clock();
    double elapsed = (double)(end - start) / CLOCKS_PER_SEC;

    printf("Found at index = %d\n", found_index);
    printf("Time with %d threads = %.6f seconds\n", NUM_THREADS, elapsed);

    free(arr);
    pthread_mutex_destroy(&lock);
    return 0;
}
