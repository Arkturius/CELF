/**
 *	strtab.h
 * -------------------------------------------------------------------------- */

#ifndef _STRTAB_H
# define _STRTAB_H

#include <celf_utils.h>

struct			_celf_strtab_s
{
	union
	{
		uint8_t	*raw;
		char	*str;
	};
	uint32_t	size;
	uint32_t	count;
};

# define	strtab_sizing(x)		((x) << 12)
# define	strtab_size_check(x)	((x) & ((1 << 12) - 1))

# define	strtab_get(tab, idx)	(&tab->str[idx])
# define	strtab_set(tab, idx, v)	tab->str[idx] = v

STRTAB		*strtab_new(uint64_t size);
void		strtab_destroy(STRTAB *tab);
uint32_t	strtab_alloc(STRTAB *tab, const char *str);

#endif // _STRTAB_H
