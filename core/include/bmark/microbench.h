#ifndef BENCH_LIBRARY_H
#define BENCH_LIBRARY_H
/**
 * @brief NUmber of warm-up batches executed before benchmarking
 *
 * Warm-up batches allow system to reach a stable state before
 * measurements are collected
 *
 */
#ifndef WARMUP_BATCH
# define WARMUP_BATCH 2
#endif
/**
 * @brief Number of maximum batches executed during benchamrking
 *
 * This limit act as a safeguards against infinite testing loops
 * when the benchmark does not converge
 *
 */
#ifndef MAX_BATCH
# define MAX_BATCH 100
#endif
/**
 * @brief Number of times the tested function is executed
 *
 */
#ifndef BATCH
# define BATCH 50
#endif
/**
 * @brief Coefficient of variation threshold used to determine convergence
 *
 * The coefficient of variation (CV) is calculated as the standard
 * deviation divided by the mean execution time.
 */
#ifndef CONVERGENCE_THRESHOLD
# define CONVERGENCE_THRESHOLD 2
#endif
/**
 * @brief The minimum number of consecutive batches with a valid CV
 *
 * The benchmark is considered stable when the convergence criterion
 * is met for this number of consecutive batches
 */
#ifndef VALID_BATCH
# define VALID_BATCH 3
#endif

#ifdef BENCH_LOG_ENABLED
# define LOG(msg) fprintf(stderr, "%s\n", msg)
#else
# define LOG(msg) ((void)0)
#endif

#include <stdbool.h>
#include <stddef.h>

typedef struct s_bench
{
    char        name[64];
    uint64_t    total_iter;
    uint64_t    min_ns;
    uint64_t    max_ns;
    double      mean_ns;
    double      median_ns;
    double      stddev_ns;
    double      p95_ns;
    double      p99_ns;
    uint32_t    batches_run;
    bool        converged;
} t_bench;

#endif
