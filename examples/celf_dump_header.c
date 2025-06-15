#include <u_flags.h>

# define	NM_FLAGS(X)							\
	X(U_FLAG_SIMPLE(NONE, 0, 'a')),				\
	X(U_FLAG_DOUBLE(NONE, 0, debug_syms)),		\

// 	X(U_FLAG_SHORT(U_NONE, 'g', 1)),
// 	X(U_FLAG_SHORT(U_NONE, 'u', 2)),
// 	X(U_FLAG_SHORT(U_NONE, 'r', 3)),
// 	X(U_FLAG_SHORT(U_NONE, 'p', 4)),
// 	X(U_FLAG_LONG(U_NONE, "extern-only", 1)),
// 	X(U_FLAG_LONG(U_NONE, "undefined-only", 2)),
// 	X(U_FLAG_LONG(U_NONE, "reverse-sort", 3)),
// 	X(U_FLAG_LONG(U_NONE, "no-sort", 4)),

U_FLAGS_GEN(nm, NM_FLAGS)

int main(UNUSED int argc, UNUSED char **argv)
{
	
}

