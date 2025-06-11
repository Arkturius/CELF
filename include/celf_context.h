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
	uint8_t		*raw;
	uint32_t	size;
	uint32_t	filename;
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
	ELF						file;
	struct _celf_strtab_s	*strtab;
};

#endif // _CELF_CONTEXT_H
