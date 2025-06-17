/**
 *	nm.c
 * -------------------------------------------------------------------------- */

#include <u_flags.h>

# define	NM_FLAGS(X)														\
	X(																		\
		NM_DISP_DEBUG, U_NONE, 'a', "debug-syms",							\
		"Display debugger-only symbols"										\
	)																		\
	X(																		\
		NM_DISP_EXTRN, U_NONE, 'g', "extern-only",							\
		"Display only external symbols"										\
	)																		\
	X(																		\
		NM_SORT_NONE, U_NONE, 'p', "no-sort",								\
		"Do not sort the symbols"											\
	)																		\
	X(																		\
		NM_SORT_REVERSE, U_NONE, 'r', "reverse-sort",						\
		"Reverse the sense of the sort"										\
	)																		\
	X(																		\
		NM_DISP_UNDEF, U_NONE, 'u', "undefined-only",						\
		"Display only undefined symbols"									\
	)																		\

U_FLAGS_GEN(nm, NM_FLAGS, "List symbols in [file(s)] (a.out by default).")

int main(UNUSED int argc, UNUSED char **argv)
{
	uint64_t	flags = nm_flags_parse(argc, argv);

	if (flags == U_FLAGS_FAIL)
		nm_usage(1);
}

