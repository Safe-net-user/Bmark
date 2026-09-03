#ifndef BMARK_BENCHMARK_MACROS_H
#define BMARK_BENCHMARK_MACROS_H

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
 * It is the relative deviation between the current batch
 * mean and the cumulative mean.
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

#endif //BMARK_MACROS_H
