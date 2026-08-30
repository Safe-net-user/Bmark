#ifndef BMARK_BENCHMARK_API_H
#define BMARK_BENCHMARK_API_H

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

#endif //BMARK_BMARK_API_H
