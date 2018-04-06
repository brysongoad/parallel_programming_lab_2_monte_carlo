#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <float.h>
#include <omp.h>

int main(int argc, char* argv[]) {
    long long int num_throws, num_hits;
    int thread_count;
    double estimate;

    num_throws = strtoll(argv[1], NULL, 10);
    thread_count = atoi(argv[2]);

    num_hits = 0;
    int seed;
    double x, y;
#   pragma omp parallel num_threads(thread_count) reduction (+:num_hits) private (seed) private(x) private (y)
    {
        seed = time(NULL)^omp_get_thread_num();
#       pragma omp for
        for (long long int i = 0; i < num_throws; ++i) {
            x = (double) rand_r(&seed) / (RAND_MAX / 2) - 1.0;
            y = (double) rand_r(&seed) / (RAND_MAX / 2) - 1.0;
            if (x * x + y * y <= 1) num_hits += 1;
        }
    }

    estimate = 4*num_hits/(double)num_throws;
    printf("estimate: %.*f\n", DBL_DECIMAL_DIG, estimate);

    return 0;
}