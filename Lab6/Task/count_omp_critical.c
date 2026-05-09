#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

int main(void)
{
    const long N = 100000000L;
    const int TARGET = 42;
    int *arr = malloc(sizeof(int) * (size_t)N);
    if (arr == NULL) {
        perror("malloc");
        return 1;
    }

    /* Fill array sequentially */
    srand((unsigned)time(NULL));
    for (long i = 0; i < N; ++i) {
        arr[i] = rand() % 100;
    }

    double t0 = omp_get_wtime();
    long global_count = 0L;

    #pragma omp parallel
    {
        long local_count = 0L;
        #pragma omp for nowait
        for (long i = 0; i < N; ++i) {
            if (arr[i] == TARGET) ++local_count;
        }

        #pragma omp critical
        {
            global_count += local_count;
        }
    }

    double t1 = omp_get_wtime();
    int threads = omp_get_max_threads();

    printf("Count: %ld\n", global_count);
    printf("Time (seconds): %f\n", t1 - t0);
    printf("Threads used: %d\n", threads);

    free(arr);
    return 0;
}
