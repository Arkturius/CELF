/**
 *	u_flags.h - command line flags helper.
 * -------------------------------------------------------------------------- */

#ifndef _UFLAGS_H
# define _UFLAGS_H

# include <u_printf.h>
# include <u_data.h>

typedef struct _u_flag_ctx	u_flag_ctx;
typedef struct _u_flag		u_flag;

struct _u_flag_ctx
{
	u_flag	*flags;
};

struct _u_flag
{
	uint64_t	type;
	char		*flag;
	union
	{
		int64_t		ival;
		char		*sval;
	};
};

enum _u_flag_type
{
	U_NONE		= 1 << 8,
	U_INT		= 1 << 9,	// TODO : Handle argument values.
	U_STRING	= 1 << 10,
	U_FLOAT		= 1 << 11,
}	u_flag_type;

typedef unsigned long long	u_flags;

# define	INLINE			static inline

# define	U_FLAGS_FAIL						-1u

# define	U_FLAG_CHK(mask, flag)				(mask & (1 << flag))

# define	U_SHIFT(ac, av)						(ac--, *av++)

# define	U_FLAGS_LIST(scope)					U_CONCAT(scope, _flaglist)

# define	U_FLAGS_ENUM_GEN(n, t, c, s, d)		n,

# define	U_FLAGS_STRUCT_GEN(n, t, c, s, d)								\
																			\
	[n] = (u_flag) { .type = t | c, .flag = s, .ival = 0},					\

# define	U_FLAGS_CASE_GEN(n, t, c, s, d)									\
																			\
	case c: { mask |= (1 << n); continue; }									\

# define	U_FLAGS_LONGOPT_GEN(n, t, c, s, d)								\
																			\
	if (!u_strcmp(flag, s)) { mask |= (1 << n); continue; }					\

# define	U_FLAGS_USAGE_GEN(n, t, c, s, d)								\
																			\
	u_printf("  -%c%s --%-16s %s\n", c, s ? "," : "", s ? s : "", d);		\


# define	U_FLAGS_HELP(scope, X)											\
	X(																		\
		U_CONCAT(scope, _HELP), U_NONE, 'h', "help",						\
		"Display this information"											\
	)																		\

# define	U_FLAGS_GEN(scope, list, desc)									\
																			\
u_flag_ctx	_uflags_ctx	= {0};												\
																			\
enum U_CONCAT(scope, _bitflags)												\
{																			\
	U_CONCAT(scope, _FIRST),												\
	list(U_FLAGS_ENUM_GEN)													\
	U_CONCAT(scope, _HELP),													\
	U_CONCAT(scope, _LAST),													\
};																			\
																			\
UNUSED																		\
static const u_flag	U_CONCAT(scope, _flaglist)[U_CONCAT(scope, _LAST)] =	\
{																			\
	[0] = (u_flag) { .type = 0, .flag = 0, .ival = 0 },						\
	list(U_FLAGS_STRUCT_GEN)												\
};																			\
																			\
INLINE void	U_CONCAT(scope, _usage)(int ret)								\
{																			\
	int	fd_out = 1 + !!ret;													\
																			\
	u_dprintf(fd_out, "Usage: %s [option(s)] [file(s)]\n", #scope);			\
	u_dprintf(fd_out, " %s\n", desc);										\
	u_dprintf(fd_out, " The options are:\n");								\
	list(U_FLAGS_USAGE_GEN)													\
	U_FLAGS_HELP(scope, U_FLAGS_USAGE_GEN)									\
	exit(ret);																\
}																			\
																			\
INLINE u_flags	U_CONCAT(scope, _flags_parse)(int argc, char **argv)		\
{																			\
	UNUSED const char	*exe = U_SHIFT(argc, argv);							\
	u_flags				mask = 0;											\
																			\
	while (argc)															\
	{																		\
		const char	*flag = U_SHIFT(argc, argv);							\
																			\
		if (*flag == '-')													\
		{																	\
			flag++;															\
			if (*flag == '-')												\
			{																\
				flag++;														\
				list(U_FLAGS_LONGOPT_GEN)									\
				U_FLAGS_HELP(scope, U_FLAGS_LONGOPT_GEN)					\
				return (-1u);												\
			}																\
			switch (*flag)													\
			{																\
				list(U_FLAGS_CASE_GEN)										\
				U_FLAGS_HELP(scope, U_FLAGS_CASE_GEN)						\
				default:													\
				{															\
					u_printf("%s: invalid option -- '%c'\n", #scope, *flag);\
					return (-1u);											\
				}															\
			}																\
			return (-1u);													\
		}																	\
	}																		\
	if (mask & (1 << U_CONCAT(scope, _HELP)))								\
		U_CONCAT(scope, _usage)(0);											\
	return (mask);															\
}																			\
																			\
INLINE int U_CONCAT(scope, _flags_check)(u_flags mask)						\
{																			\
	return (mask != U_FLAGS_FAIL);											\
}																			\

#endif // _UFLAGS_H
