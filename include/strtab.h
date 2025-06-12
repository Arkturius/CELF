/**
 *	strtab.h
 *
 *		String table, as they are in ELF files.
 *		
 *		strtab_new() uses the mmap() syscall to allocate tables. Actually, you
 *		cant't create a String table with something else than a multiple of
 *		0x1000.
 *
 *		strtab_array() uses the hooks defined by the macros:
 *			strtab_malloc(uint64_t size);
 *			strtab_free(void *ptr);
 *		They are set by default to be respectively malloc and free, but they are
 *		up to you to replace.
 * -------------------------------------------------------------------------- */

#ifndef _STRTAB_H
# define _STRTAB_H

# include <celf_utils.h>

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

# define	strtab(r, s)			((STRTAB){ .raw = r, .size = s})

# define	strtab_malloc(x)		malloc(x)
# define	strtab_free(x)			free(x)

STRTAB		*strtab_new(uint32_t size);
void		strtab_destroy(STRTAB *tab);

const char	**strtab_array(STRTAB *tab);

uint32_t	strtab_alloc(STRTAB *tab, const char *str);

#endif // _STRTAB_H
