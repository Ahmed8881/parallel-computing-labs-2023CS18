#include <stdio.h>
#include <omp.h>

int main(void)
{
    /* Simple OpenMP hello world: each thread prints its id and total */
    #pragma omp parallel
    {
        int tid = omp_get_thread_num();
        int nthreads = omp_get_num_threads();
        printf("Hello from thread %d of %d\n", tid, nthreads);
    }

    return 0;
}
