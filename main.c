/* Bryson Goad
 * 4/7/2018
 * Parallel Programming lab 2 - Monte Carlo
 * Estimates pi using the Monte Carlo method parallelized with OpenMP
 * Program Arguments: <
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <float.h>
#include <omp.h>

int main(int argc, char* argv[]) {
    long long int iteration_count,  // number of iterations used, input as program argument
            hit_count,              // number of iterations that "hit" the circle
            i;                      // loop index;
    int thread_count,               // number of threads used, input as program argument
            seed;                   // seed for random number generation
    double estimate,                // estimate of pi
            x, y,                   // x and y values for each iteration
            time_start, time_end;   // start and end times for parallel section

    // initialize with program arguments
    iteration_count = strtoll(argv[1], NULL, 10);
    thread_count = atoi(argv[2]);

    hit_count = 0;
    // start timer
    time_start = omp_get_wtime();
    // determine the number of hits using parallelism
#   pragma omp parallel num_threads(thread_count) reduction (+:hit_count) private (seed, x, y, i)
    {
        seed = time(NULL)^omp_get_thread_num(); // generate unique seed for each thread
#       pragma omp for
        for (i = 0; i < iteration_count; ++i) {
            // randomly generate xa nd y values between -1.0 and 1.0
            x = (double) rand_r(&seed) / (RAND_MAX / 2) - 1.0;
            y = (double) rand_r(&seed) / (RAND_MAX / 2) - 1.0;
            // if "hit" increment hit count
            if (x * x + y * y <= 1) hit_count++;
        }
    }
    // end timer
    time_end = omp_get_wtime();

    // calculate estimate
    estimate = 4*hit_count/(double)iteration_count;

    // display estimate and elapsed time of the parallel section
    printf("estimate: %.*f\n", DBL_DECIMAL_DIG, estimate);
    printf("elapsed time: %f seconds\n", time_end - time_start);

    return 0;
}