#ifndef BMARK_BENCHMARK_API_H
#define BMARK_BENCHMARK_API_H

#include "types.h"

t_bench	*create_bench(char *name);
t_rv	bench_run(t_bench *bench, void (*f)(void));
void	bench_print(t_bench *bench);

#endif //BMARK_BMARK_API_H
