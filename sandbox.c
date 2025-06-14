#include <stdint.h>
#include <stdio.h>

#include <ft_nm.h>

static char	nm_fetchSymbolLetter(ELF64_Sym *sym)
{
	char	letter = 0;

	uint16_t	section_index = READ_FIELD(sym->st_shidx);
	uint8_t		symbol_info = READ_FIELD(sym->st_info);	
	uint8_t		symbol_type = ELF64_ST_TYPE(symbol_info);
	uint8_t		symbol_bind = ELF64_ST_BIND(symbol_info);

	letter = '?';
	if (symbol_bind == STB_WEAK)
	{
		if (symbol_type == STT_OBJECT)
			letter = 'V';
		else
			letter = 'W';
		if (section_index == SHN_UNDEF)
			letter |= 32;
	}
	else if (section_index == SHN_UNDEF)
		letter = 'U';
	else if (section_index == SHN_ABS)
		letter = 'A';
	else if (section_index == SHN_COMMON)
		letter = 'C';
	else
	{
		if (symbol_type == STT_GNU_IFUNC)
			letter = 'I';
		else
		{
			ELF64_Shdr	*section = ((ELF64_Shdr *)ELF_CONTEXT.s_headers) + section_index;
			uint64_t	section_flags = READ_FIELD(section->sh_flags);
			uint32_t	section_type = READ_FIELD(section->sh_type);


			if (section_flags & SHF_EXECINSTR)
				letter = 'T';
			else if (section_flags & SHF_WRITE)
			{
				letter = 'D';
				if (section_type == SHT_NOBITS)
					letter = 'B';
			}
			else if (section_flags & SHF_ALLOC)
				letter = 'R';
			else if (section_type == SHT_PROGBITS)
				letter = 'N';
		}
	}	
	if (symbol_bind == STB_LOCAL && letter >= 'A' && letter <= 'Z')
		letter |= 32;
	return (letter);
}

#include <string.h>

int main(UNUSED int argc, UNUSED char **argv)
{
	ELF_open(argv[0]);

	getSections();
	
	ELF64_Shdr	*symtab = getSectionByName(".symtab");
	uint32_t	link = READ_FIELD(symtab->sh_link);

	char		*names = (char *) getSectionContent(getSectionByIndex(link));

	ELF64_Sym	*symbols = getSymbols();
	uint32_t	scount = getSymbolCount();

	char		**names_tab = malloc(sizeof(char *) * scount);
	ELF64_Sym	**symbols_tab = malloc(sizeof(ELF64_Sym *) * scount);
	for (uint32_t i = 0; i < scount; ++i)
	{
		uint32_t	idx = READ_FIELD(symbols[i].st_name);
		names_tab[i] = names + idx;
		symbols_tab[i] = &symbols[i];
	}
	
	uint8_t swapped;
	for (uint32_t i = 0; i < scount - 1; ++i)
	{
		swapped = 0;
		for (uint32_t j = 0; j < scount - 1 - i; ++j)
		{
			if (strcmp((const char *)names_tab[j], (const char *)names_tab[j + 1]) > 0)
			{
				char	*tmp = names_tab[j];
				
				names_tab[j] = names_tab[j + 1];
				names_tab[j + 1] = tmp;

				ELF64_Sym	*tmps = symbols_tab[j];
				
				symbols_tab[j] = symbols_tab[j + 1];
				symbols_tab[j + 1] = tmps;

				swapped = 1;
			}
		}
		if (!swapped)
			break;
	}

	for (uint32_t i = 0; i < scount; ++i)
	{
		if (ELF64_ST_TYPE(READ_FIELD(symbols_tab[i]->st_info)) == STT_NOTYPE && !*names_tab[i])
			continue ;

		char	letter = nm_fetchSymbolLetter(symbols_tab[i]);

		uint64_t	val = READ_FIELD(symbols_tab[i]->st_value);
		
		val = (letter == 'U' || letter == 'w') ? -1UL : val;
		if (val == -1UL)
			printf("                ");
		else
			printf("%016lx", val);
		printf(" %c %s\n", letter, names_tab[i]);
	}

//	ELF_close();
}
