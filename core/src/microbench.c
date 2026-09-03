#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include "../include/bmark/benchmark.h"

t_bench	*create_bench(char *name)
{
	t_bench	*b;

	if (!name)
		return (NULL);
	if (strlen(name) >= 64)
	{
		fprintf(stderr, "bmark: create_bench: the identifier name must be less than 64 characters\n");
		return (NULL);
	}
	b = malloc(sizeof(t_bench));
	if (!b)
	{
		fprintf(stderr, "bmark: create_bench: bad allocation");
		return (NULL);
	}
	strcpy(b->name, name);
	b->total_iter = 0;
	b->min_ns = 0;
	b->max_ns = 0;
	b->mean_ns = 0;
	b->median_ns = 0;
	b->stddev_ns = 0;
	b->p95_ns = 0;
	b->p99_ns = 0;
	b->p999_ns = 0;
	b->batches_run = 0;
	b->converged = 0;
	return (b);
}

static int compare(const void *a, const void *b)
{
	uint64_t x = *(const uint64_t *)a;
	uint64_t y = *(const uint64_t *)b;

	if (x < y)
		return (-1);
	if (x > y)
		return (1);
	return (0);
}

static double percentile(uint64_t *samples, size_t n, double percentile)
{
	size_t index;

	index = (size_t)ceil(percentile * (double)n) - 1;
	return (double)samples[index];
}

t_rv	bench_run(t_bench *bench, void (*f)(void))
{
	struct timespec	start;
	struct timespec	end;

	size_t			batch_count = 0;
	size_t			index_batch = 0;

	uint64_t		total_iter = 0;
	uint64_t		elapsed_ns = 0;
	uint64_t		all_elapsed = 0;
	uint64_t		max_ns = 0;
	uint64_t		min_ns = 0;
	uint64_t		*all_measures;
	double			batch_mean = 0.0;
	double			batches_mean = 0.0;
	double			final_batches_mean = 0.0;
	short			valid_batch = 0;

	double			mesure = 0.0;
	uint64_t		i = 0;

	#ifdef WARMUP_BATCH
		LOG("[bench] warm-up started");
		while (batch_count < WARMUP_BATCH)
		{
			while (index_batch < BATCH)
			{
				f();
				index_batch++;
			}
			index_batch = 0;
			batch_count++;
		}
		LOG("[bench] warm-up finished");
	#endif
	LOG("[bench] benchmark started");
	all_measures = malloc(sizeof(uint64_t) * (MAX_BATCH * BATCH));
	if (!all_measures)
		return B_BAD_ALLOCATION;
	batch_count = 0;
	while (valid_batch < VALID_BATCH && batch_count < MAX_BATCH)	{
		index_batch = 0;
		all_elapsed = 0;
		while (index_batch < BATCH)
		{
			clock_gettime(CLOCK_MONOTONIC_RAW, &start);
			f();
			clock_gettime(CLOCK_MONOTONIC_RAW, &end);
			elapsed_ns = ((uint64_t)end.tv_sec - (uint64_t)start.tv_sec) * 1000000000ULL + ((uint64_t)end.tv_nsec - (uint64_t)start.tv_nsec);
			set_min_max(elapsed_ns, &min_ns, &max_ns);
			all_elapsed += elapsed_ns;
			all_measures[total_iter] = elapsed_ns;
			index_batch++;
			total_iter++;
		}
		batch_mean = all_elapsed / BATCH;
		if (batch_count != 0)
		{
			if (fabs(batch_mean - final_batches_mean) / final_batches_mean * 100.0 <= CONVERGENCE_THRESHOLD)
				valid_batch++;
		}
		else
			valid_batch = 0;
		batches_mean += batch_mean;
		final_batches_mean = batches_mean / (batch_count + 1);
		batch_count++;
	}
	qsort(all_measures, total_iter, sizeof(uint64_t), compare);
	bench->total_iter = total_iter;
	bench->min_ns = min_ns;
	bench->max_ns = max_ns;
	bench->mean_ns = final_batches_mean;
	bench->median_ns = all_measures[total_iter / 2];
	while (i < total_iter)
	{
		mesure += (all_measures[i] - final_batches_mean) * (all_measures[i] - final_batches_mean);
		i++;
	}
	bench->stddev_ns = sqrt(mesure / (total_iter - 1));
	bench->p95_ns = percentile(all_measures, total_iter, 0.95);
	bench->p99_ns = percentile(all_measures, total_iter, 0.99);
	bench->p999_ns = percentile(all_measures, total_iter, 0.999);
	bench->batches_run = batch_count;
	bench->converged = valid_batch == VALID_BATCH;
	LOG("[bench] benchmark finished");
	free(all_measures);
	return (B_SUCCESS);
}

void	bench_print(t_bench *bench)
{
	printf("Benchmark: %s\n", bench->name);
	printf("  Iterations : %lu\n", bench->total_iter);
	printf("  Batches    : %lu\n", bench->batches_run);
	printf("  Converged  : %s\n", bench->converged ? "yes" : "no");
	printf("\n");
	printf("  Min        : %.lu ns\n", bench->min_ns);
	printf("  Max        : %.lu ns\n", bench->max_ns);
	printf("  Mean       : %.2f ns\n", bench->mean_ns);
	printf("  Median     : %.lu ns\n", bench->median_ns);
	printf("  Stddev     : %.2f ns\n", bench->stddev_ns);
	printf("  P95        : %.lu ns\n", bench->p95_ns);
	printf("  P99        : %.lu ns\n", bench->p99_ns);
	printf("  P99.9      : %.lu ns\n", bench->p999_ns);
}