/**
 *	celf_impl.h | CELF implementation template.
 * -------------------------------------------------------------------------- */

#if !defined(_CELF_H)
# error "This file should not be included alone."
#endif

#if !defined(CELF_CLASS)
# error "CELF_CLASS is not defined."
#endif

#include <celf_utils.h>

#define	CELF_UINT	CONCAT(CONCAT(uint, CELF_CLASS), _t)

/**
 *	@brief	ELF_header_get()
 *
 *	@return	Pointer to the ELF header.
 * -------------------------------------------------------------------------- */
CELF(Hdr)	*CELF(header_get)(void)
{
	if (!CELF_CTX.header)
	{
		uint8_t	*header = ELF_OFFSET(0);

		if (ELF_PTR_CHECK(header))
			CELF_THROW(_celf_fail_ptrchk);

		CELF_CTX.header = header;
	}
	return ((CELF(Hdr) *) CELF_CTX.header);
}

/**
 *	@brief	ELF_sheaders_get()
 *
 *	@return	Pointer to the section headers array.
 * -------------------------------------------------------------------------- */
CELF(Shdr)	*CELF(sheaders_get)(void)
{
	if (!CELF_CTX.sheaders)
	{
		CELF(Hdr)	*header = CELF(header_get)();

		CELF_UINT	offset = READ_FIELD(header->e_shoff);
		uint8_t		*sheader = ELF_OFFSET(offset);

		if (ELF_PTR_CHECK(sheader))
			CELF_THROW(_celf_fail_ptrchk);

		CELF_CTX.sheaders = sheader;
	}
	return ((CELF(Shdr) *) CELF_CTX.sheaders);
}

/**
 *	@brief	ELF_sheaders_size()
 *
 *	@return	Size of section headers array (section count).
 * -------------------------------------------------------------------------- */
uint16_t	CELF(sheaders_size)(void)
{
	if (!CELF_CTX.sheaders_size)
	{
		CELF(Hdr)	*header = CELF(header_get)();

		CELF_CTX.sheaders_size = READ_FIELD(header->e_shnum);
	}
	return (CELF_CTX.sheaders_size);
}

/**
 *	@brief	ELF_pheaders_get()
 *
 *	@return	Pointer to the program headers array.
 * -------------------------------------------------------------------------- */
CELF(Phdr)	*CELF(pheaders_get)(void)
{
	if (!CELF_CTX.pheaders)
	{
		CELF(Hdr)	*header = CELF(header_get)();

		CELF_UINT	offset = READ_FIELD(header->e_phoff);
		uint8_t		*pheader = ELF_OFFSET(offset);

		if (ELF_PTR_CHECK(pheader))
			CELF_THROW(_celf_fail_ptrchk);

		CELF_CTX.pheaders = pheader;
	}
	return ((CELF(Phdr) *) CELF_CTX.pheaders);
}

/**
 *	@brief	ELF_pheaders_size()
 *
 *	@return	Size of program headers array (header count).
 * -------------------------------------------------------------------------- */
uint16_t	CELF(pheaders_size)(void)
{
	if (!CELF_CTX.pheaders_size)
	{
		CELF(Hdr)	*header = CELF(header_get)();

		CELF_CTX.pheaders_size = READ_FIELD(header->e_phnum);
	}
	return (CELF_CTX.pheaders_size);
}

/**
 *	@brief	ELF_section_content()
 *
 *	@return	Raw pointer to section content.
 * -------------------------------------------------------------------------- */
uint8_t	*CELF(section_content)(CELF(Shdr) *section)
{
	CELF_UINT	offset = READ_FIELD(section->sh_offset);
	uint8_t		*content = ELF_OFFSET(offset);

	if (ELF_PTR_CHECK(content))
		CELF_THROW(_celf_fail_ptrchk);

	return (content);
}

/**
 *	@brief	ELF_section_name()
 *
 *	@return	Raw pointer to section name inside of shstrtab.
 * -------------------------------------------------------------------------- */
const char	*CELF(section_name)(CELF(Shdr) *section)
{
	char		*shstrtab = CELF(shstrtab_get)();
	uint32_t	name_idx = READ_FIELD(section->sh_name);
	
	const char	*name = shstrtab + name_idx;

	return (name);
}

/**
 *	@brief	ELF_shstrtab_get()
 *
 *	@return	char pointer to the section names strtab.
 * -------------------------------------------------------------------------- */
char	*CELF(shstrtab_get)(void)
{
	if (!CELF_CTX.shstrtab)
	{
		CELF(Hdr)	*header = CELF(header_get)();
		uint16_t	shstridx = READ_FIELD(header->e_shstridx);

		CELF(Shdr)	*sheaders = CELF(sheaders_get)();
		CELF(Shdr)	*strtabhdr = &sheaders[shstridx];

		char	*shstrtab = (char *) CELF(section_content)(strtabhdr);

		CELF_CTX.shstrtab = shstrtab;
	}
	return (CELF_CTX.shstrtab);
}

/**
 *	@brief	ELF_sheader_get_by_name(const char *target)
 *
 *	@return	Pointer to the section header identified by 'target', NULL if
 *			not found.
 * -------------------------------------------------------------------------- */
CELF(Shdr)	*CELF(sheader_get_by_name)(const char *target)
{
	CELF(Shdr)	*sheaders = CELF(sheaders_get)();
	uint16_t	sheaders_size = CELF(sheaders_size)();
	char		*shstrtab = CELF(shstrtab_get)();

	for (uint32_t i = 0; i < sheaders_size; ++i)
	{
		CELF(Shdr)	*it = &sheaders[i];
		uint32_t	name_idx = READ_FIELD(it->sh_name);
		char		*name = shstrtab + name_idx;

		if (u_strcmp(name, target))
			continue ;
		return (it);
	}
	return (NULL);
}

/**
 *	@brief	ELF_sheader_get_by_idx(uint16_t n)
 *
 *	@return	Pointer to the n-th section header.
 * -------------------------------------------------------------------------- */
CELF(Shdr)	*CELF(sheader_get_by_idx)(uint16_t n)
{
	CELF(Shdr)	*sheaders = CELF(sheaders_get)();
	uint16_t	sheaders_size = CELF(sheaders_size)();

	if (n >= sheaders_size)
		return (NULL);

	CELF(Shdr)	*section = sheaders + n;

	return (section);
}

CELF(Sym)	*CELF(symtab_get)(void)
{
	if (!CELF_CTX.symtab)
	{
		CELF(Shdr)	*symtab_hdr = CELF(sheader_get_by_name)(".symtab");

		CELF_CTX.symtab = (CELF(Sym) *)CELF(section_content)(symtab_hdr);
	}
	return ((CELF(Sym) *)CELF_CTX.symtab);
}

uint64_t	CELF(symtab_size)(void)
{
	if (!CELF_CTX.symtab_size)
	{
		CELF(Shdr)	*symtab_hdr = CELF(sheader_get_by_name)(".symtab");

		if (!symtab_hdr)
			return (0);

		CELF_UINT	fullsize = READ_FIELD(symtab_hdr->sh_size);
		CELF_UINT	entsize = READ_FIELD(symtab_hdr->sh_entsize);

		if (entsize == 0)
			CELF_THROW(_celf_fail_zerodiv);

		CELF_CTX.symtab_size = (uint64_t) fullsize / entsize;
	}
	return (CELF_CTX.symtab_size);
}
