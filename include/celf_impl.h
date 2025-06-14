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

uint16_t	CELF(sheaders_size)(void)
{
	if (!CELF_CTX.sheaders_size)
	{
		CELF(Hdr)	*header = CELF(header_get)();

		CELF_CTX.sheaders_size = READ_FIELD(header->e_shnum);
	}
	return (CELF_CTX.sheaders_size);
}

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

uint8_t	*CELF(section_content)(CELF(Shdr) *section)
{
	CELF_UINT	offset = READ_FIELD(section->sh_offset);
	uint8_t		*content = ELF_OFFSET(offset);

	if (ELF_PTR_CHECK(content))
		CELF_THROW(_celf_fail_ptrchk);

	return (content);
}

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

CELF(Sym)	*CELF(symtab_get)(void)
{
	return (NULL);
}

CELF_UINT	CELF(symtab_size)()
{
	return (0);
}
