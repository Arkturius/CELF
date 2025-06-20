/**
 *	celf.c | CELF implementation.
 * -------------------------------------------------------------------------- */

#include <celf.h>

CELF _celf_ctx = {0};

CELF_API(int, ELF_open, const char *filename)
{
	int	fd = open(filename, O_RDONLY);

	if (fd == -1)
		return (1);

	struct stat	st;
	u_memset(&st, 0, sizeof(struct stat));

	if (fstat(fd, &st) == -1)
		return (1);

	void *ptr = mmap(NULL, st.st_size, PROT_READ, MAP_PRIVATE, fd, 0);

	if (ptr == MAP_FAILED)
		return (1);
	close(fd);

	ELF_RAW = (uint8_t *)ptr;
	ELF_SIZE = (uint32_t)st.st_size;
	ELF_FILENAME = filename;

	return (0);
}

CELF_API(DESTRUCTOR void, ELF_close)
{
	if (ELF_RAW != MAP_FAILED)
		munmap(ELF_RAW, ELF_SIZE);
}

CELF_API(int, ELF_check)
{
	if (ELF_SIZE < ELF_MINSIZE)
		return (1);
	
	ELF_Ident	ident = *(ELF_Ident *)ELF_RAW;

	uint32_t	sig = *(uint32_t *)&ident.e_magic[0];

	if (sig != ELF_MAGIC)
	{
		CELF_ERROR(_celf_invalid_magic);
		return (1);
	}
	if (!ident.e_class || (ident.e_class & ~3))
	{
		CELF_ERROR(_celf_invalid_class);
		return (1);
	}
	if (ident.e_endianness & ~3)
	{
		CELF_ERROR(_celf_invalid_endian);
		return (1);
	}
	if (ident.e_version != 1)
	{
		CELF_ERROR(_celf_invalid_version);
		return (1);
	}
	if (ident.e_ABI > 0x12)
	{
		CELF_ERROR(_celf_invalid_ABI);
		return (1);
	}
	return (0);
}

CELF_API(inline int, ELF_is64bit)
{
	return (ELF_RAW[EI_CLASS] == ELF_64BIT);
}

CELF_API(inline int, ELF_is32bit)
{
	return (ELF_RAW[EI_CLASS] == ELF_32BIT);
}

# define	CELF_CLASS	64
# include <celf_impl.h>
# undef		CELF_CLASS

# define	CELF_CLASS	32
# include <celf_impl.h>
# undef		CELF_CLASS
