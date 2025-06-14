#include <celf.h>

#define	SHIFT(ac, av)	(ac--, *av++)

const char	*exe;

void	usage(int err)
{
	u_dprintf(STDERR_FILENO, "%s: No file provided\n", exe);
	exit(err);
}

typedef struct
{
	const char	*name;
	uint64_t	value : 56;
	uint64_t	c : 8;
}	SYM;

typedef struct
{
	uint32_t	count;
	SYM			table[];
}	SYMTAB;

int main(UNUSED int argc, UNUSED char **argv)
{
	exe = SHIFT(argc, argv);

	if (argc != 1)
		usage(1);

	UNUSED const char	*filename = SHIFT(argc, argv);

	ELF_open(filename);
	if (!ELF_check())
	{
		if (ELF_is64bit())
		{
		}
		else
		{
		}
	}	
 	ELF_close();
}
