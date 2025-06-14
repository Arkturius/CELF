/**
 *	strtab.h
 *
 *		Symbol table.
 */

#ifndef _SYMTAB_H
# define _SYMTAB_H

# include <celf_utils.h>
# include <strtab.h>

struct _celf_symtab
{
	uint64_t	size;
	ELF64_Hdr	*symbols[];
}	SYMTAB;

# define	SYMTAB_HDR(syms)	(((SYMTAB *)(syms)) - 1)

CELF_API(void, symtab_create, SYMTAB syms);
CELF_API(void, symtab_destroy, SYMTAB syms);

enum
{
	SYM_SORT_NONE		= 1 << 0,
	SYM_SORT_REV		= 1 << 1,

	SYM_SORT_LAST		= 1 << 7,
}

enum
{
	SYM_STYLE_HEXDUMP	= 1 << 0
	SYM_STYLE_NM		= 1 << 1,
	SYM_STYLE_READELF	= 1 << 2,

	SYM_STYLE_LAST		= 1 << 7,
}

CELF_API(void, symtab_sort, SYMTAB syms, uint8_t mode);
CELF_API(void, symtab_dump, SYMTAB syms, uint8_t style);
