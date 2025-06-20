/**
 *	u_algos.h - algorithms.
 * -------------------------------------------------------------------------- */

#ifndef _UALGOS_H
# define _UALGOS_H

# include <stdint.h>

typedef int	(*u_comparator)(const void *, const void *);

void u_qsort(void *base, uint32_t nmemb, uint32_t size, u_comparator cmp);

#endif // _UALGOS_H
