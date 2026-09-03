#ifndef BMARK_UTILS_H
#define BMARK_UTILS_H

#include <stdint.h>

static inline void set_min_max(uint64_t elapsed_ns, uint64_t *min, uint64_t *max)
{
	if (*min == 0)
	{
		*min = elapsed_ns;
		*max = elapsed_ns;
		return ;
	}
	if (*min > elapsed_ns)
		*min = elapsed_ns;
	else if (*max < elapsed_ns)
		*max = elapsed_ns;
}

#endif //BMARK_UTILS_H
