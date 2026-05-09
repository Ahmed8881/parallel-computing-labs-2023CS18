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

    double t_init0 = omp_get_wtime();

    /* Parallel initialization using rand_r with a per-thread seed */
    #pragma omp parallel
    {
        unsigned int seed = (unsigned int)(omp_get_thread_num() * 12345 + (unsigned)time(NULL));
        #pragma omp for
        for (long i = 0; i < N; ++i) {
            arr[i] = rand_r(&seed) % 100;
        }
    }

    double t_init1 = omp_get_wtime();

    double t_count0 = omp_get_wtime();
    long global_count = 0L;
    #pragma omp parallel for reduction(+:global_count)
    for (long i = 0; i < N; ++i) {
        if (arr[i] == TARGET) ++global_count;
    }
    double t_count1 = omp_get_wtime();

    double t_total0 = t_init0;
    double t_total1 = t_count1;

    int threads = omp_get_max_threads();

    printf("Count: %ld\n", global_count);
    printf("Init time (seconds): %f\n", t_init1 - t_init0);
    printf("Count time (seconds): %f\n", t_count1 - t_count0);
    printf("Total time (seconds): %f\n", t_total1 - t_total0);
    printf("Threads used: %d\n", threads);

    free(arr);
    return 0;
}
