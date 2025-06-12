#define	CELF_STRIP_PREFIXES
#include <celf.h>
#include <strtab.h>

#define	SHIFT(ac, av)	(ac--, *av++)

const char	*exe;

void	usage(int err)
{
	u_dprintf(STDERR_FILENO, "%s: No file provided\n", exe);
	exit(err);
}

int main(UNUSED int argc, UNUSED char **argv)
{
	exe = SHIFT(argc, argv);

	if (argc != 1)
		usage(1);

	UNUSED const char	*filename = SHIFT(argc, argv);

	u_printf("ELF header dump:\n");

	CELF_STRTAB = strtab_new(strtab_sizing(1));
	
	ELF_open(filename);
	if (!ELF_check())
	{
		ELF_Ident	*ident = (ELF_Ident *)ELF_RAW;

		if (ELF_is64Bit())
		{
			ELF64_Hdr	*hdr64 = (ELF64_Hdr *)ident;

			u_printf("%s\n", ELF64_Hdr_stringify(hdr64));
		}
		else
		{
//			ELF32_Hdr	*hdr32 = (ELF32_Hdr *)ident;

//			u_printf("%s", ELF32_Hdr_stringify(hdr32));
		}
	}	
 	ELF_close();
	
	strtab_destroy(CELF_STRTAB);
}
