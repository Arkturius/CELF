/**
 *	u_algos.c - algorithms.
 *
 *		contains: u_qsort()
 * -------------------------------------------------------------------------- */

#include <stdlib.h>

#include <u_algos.h>
#include <u_printf.h>

static void	*_tmp;

static void __attribute__((destructor)) u_qsort_cleanup(void)
{
	free(_tmp);
	_tmp = NULL;
}

static void	u_memswap(void *a, void *b, uint32_t size)
{
	if (a == b)
		return ;
	if (!_tmp)
		_tmp = malloc(size);
	if (!_tmp)
		return ;

	u_memcpy(_tmp, a, size);
	u_memcpy(a, b, size);
	u_memcpy(b, _tmp, size);
}

static void	u_partition(void *base, void *start, void *end, uint32_t size, void **pivot, u_comparator compare)
{
	void	*spivot = *pivot;
	void	*j = start;

	u_memswap(spivot, end, size);
	for (void *it = start; it < end; it += size)
	{
		if (compare(it, end) < 0)
		{
			u_memswap(j, it, size);
			j = (uint8_t *)j + size;
		}
	}
	u_memswap(end, j, size);
	*pivot = j;
}

static void u_rqsort(void *base, void *start, void *end, uint32_t size, u_comparator compare)
{
	if (start < end)
	{
		void	*pivot = (uint8_t *)start + (((end - start) / size) >> 1) * size;

		u_partition(base, start, end, size, &pivot, compare);
		u_rqsort(base, start, pivot - size, size, compare);
		u_rqsort(base, pivot + size, end, size, compare);
	}
}

void u_qsort(void *base, uint32_t nmemb, uint32_t size, u_comparator compare)
{
	if (!base || !compare || !nmemb || !size)
		return ;

	uint32_t	arr_size = size * nmemb;

	if (arr_size / size != nmemb)
		return ;

	void	*end = (uint8_t *)base + size * (nmemb - 1);

	u_rqsort(base, base, end, size, compare);
	u_qsort_cleanup();
}
