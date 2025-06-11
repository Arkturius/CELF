#include <u_printf.h>

#define	CELF_IMPLEMENTATION
#define	CELF_STRIP_PREFIXES
#include <celf.h>

#define	SHIFT(ac, av)	(ac--, *av++)

void	usage(int err)
{
	u_dprintf(STDERR_FILENO, "dump_header: No file provided\n");
	_exit(err);
}

#define	CELF_OPEN(filename)
#define	CELF_CLOSE()

int main(UNUSED int argc, UNUSED char **argv)
{
	UNUSED const char	*name = SHIFT(argc, argv);

	if (argc != 1)
		usage(1);

	UNUSED const char	*filename = SHIFT(argc, argv);

	u_printf("ELF header dump:\n");
	CELF_OPEN(filename);

// 	CELF_Hdr	header = {0};
// 	getELFHdr(&header);

	CELF_CLOSE();

}
