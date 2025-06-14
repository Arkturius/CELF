/**
 *	celf_context.h | CELF context header.
 * -------------------------------------------------------------------------- */

#ifndef _CELF_CONTEXT_H
# define _CELF_CONTEXT_H

# include <celf_utils.h>

/**
 *	@struct		ELF.
 *
 *	@brief			ELF file handle.
 *
 *		filename
 *		raw			raw bytes pointer to where the file is mapped
 *		size		size of the file in bytes
 */
struct			_elf_s
{
	const char	*filename;
	uint8_t		*raw;
	uint32_t	size;
};

/**
 *	@struct		CELF.
 *
 *	@brief			CELF context.
 *
 *		file		Current file handle.
 */
struct			_celf_ctx_s
{
	ELF		file;
	void	*header;

	void		*sheaders;
	uint64_t	sheaders_size;

	void		*pheaders;
	uint64_t	pheaders_size;

	void		*symtab;
	uint64_t	symtab_size;

	void	*shstrtab;
};

#endif // _CELF_CONTEXT_H
