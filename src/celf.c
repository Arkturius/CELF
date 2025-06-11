/**
 *	celf.c | CELF implementation.
 * -------------------------------------------------------------------------- */

#include <celf.h>
#include <strtab.h>

CELF _celf_ctx = {0};

CELF_API(void, ELF_open, const char *filename)
{
	int	fd = open(filename, O_RDONLY);

	if (fd == -1)
		CELF_THROW(_celf_fail_open);

	struct stat	st;
	u_memset(&st, 0, sizeof(struct stat));

	if (fstat(fd, &st) == -1)
		CELF_THROW(_celf_fail_stat);

	void *ptr = mmap(NULL, st.st_size, PROT_READ, MAP_PRIVATE, fd, 0);

	if (ptr == MAP_FAILED)
		CELF_THROW(_celf_fail_mmap);
	close(fd);

	ELF_RAW = (uint8_t *)ptr;
	ELF_SIZE = (uint32_t)st.st_size;
	ELF_FILENAME = strtab_alloc(CELF_STRTAB, filename);
}

CELF_API(DESTRUCTOR void, ELF_close)
{
	if (ELF_RAW != MAP_FAILED)
		munmap(ELF_RAW, ELF_SIZE);
}
