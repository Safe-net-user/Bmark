#ifndef BMARK_TYPES_H
#define BMARK_TYPES_H

#include <stdbool.h>
#include <stdint.h>

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
	double		p999_ns;
	uint32_t    batches_run;
	bool        converged;
} t_bench;

typedef enum e_return_value {B_SUCCESS, B_ERROR, B_BAD_ALLOCATION} t_rv;

#endif //BMARK_ENUM_H
