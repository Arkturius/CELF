/**
 *	strtab.c
 * -------------------------------------------------------------------------- */

#include <strtab.h>

STRTAB	*strtab_new(uint32_t size)
{
	if (strtab_size_check(size))
		return (NULL);

	STRTAB	*ptr = mmap
	(
		0, size, PROT_READ | PROT_WRITE,
		MAP_ANONYMOUS | MAP_PRIVATE, 0, 0
	);
	if (ptr == MAP_FAILED)
		CELF_THROW(_celf_fail_mmap);

	ptr->raw = (uint8_t *)(ptr + 1);
	ptr->size = size - sizeof(STRTAB);
	ptr->count = 1;

	u_memset(ptr->raw, 0, ptr->size);

	return (ptr);
}

void strtab_destroy(STRTAB *tab)
{
	munmap(tab, tab->size);
}

uint32_t	strtab_alloc(STRTAB *tab, const char *str)
{
	if (!tab)
		return (-1U);

	uint32_t	len = u_strlen(str);
	
	if (tab->count + len > tab->size)
		return (-1U);

	uint32_t	pos = tab->count;

	u_strcat(strtab_get(tab, pos), str);
	tab->count += len;
	strtab_set(tab, tab->count++, 0);

	return (pos);
}

const char	**strtab_array(STRTAB *tab)
{
	uint32_t	n = 0;
	char		*tmp = tab->str + 1;

	while (tmp < tab->str + tab->count)
	{
		uint32_t	len = u_strlen(tmp);

		tmp += len + 1;
		n++;
	}

	const char	**arr = strtab_malloc((n + 1) * sizeof(char *));

	if (!arr)
		return (NULL);

	tmp = tab->str + 1;
	for (uint32_t i = 0; i < n; ++i)
	{
		arr[i] = tmp;
		tmp += u_strlen(tmp) + 1;
	}
	arr[n] = NULL;
	return (arr);
}

