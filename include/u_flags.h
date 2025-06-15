/**
 *	u_flags.h - command line flags helper.
 * -------------------------------------------------------------------------- */

#ifndef _UFLAGS_H
# define _UFLAGS_H

# include <u_printf.h>
# include <u_data.h>

# define	u_FlagSpecList(X)												\
	X(U_FLAG_ATYPE_NONE,	1 << 8)											\
	X(U_FLAG_ATYPE_INT,		1 << 9)											\
	X(U_FLAG_ATYPE_STRING,	1 << 10)										\
	X(U_FLAG_DTYPE_SIMPLE,	1 << 16)										\
	X(U_FLAG_DTYPE_DOUBLE,	1 << 17)										\

# define	ENUM_TYPE	u_FlagSpec
# define	ENUM_LIST	u_FlagSpec
# include <u_stringify.h>

# define	U_NONE		U_FLAG_ATYPE_NONE
# define	U_STRING	U_FLAG_ATYPE_STRING
# define	U_INTEGER	U_FLAG_ATYPE_INT

# define	U_FLAG_POS_MASK(X)		(((X) & 0x3F) << 24)

typedef struct _u_flag
{
	u_FlagSpec	type;
	char		*flag;
	union
	{
		int64_t		ival;
		char		*sval;
	};
}	u_flagdef;

#define U_SIMPLE_FLAG_DEF(argtype, bit, letter)								\
	{																		\
		.type =																\
			(letter)														\
			| U_FLAG_DTYPE_SIMPLE											\
			| U_FLAG_ATYPE_##argtype										\
			| U_FLAG_POS_MASK(bit),											\
		.flag = 0,															\
		.ival = 0															\
	}																		\
#define U_DOUBLE_FLAG_DEF(argtype, bit, name)								\
	{																		\
		.type =																\
			U_FLAG_DTYPE_DOUBLE												\
			| U_FLAG_ATYPE_##argtype										\
			| U_FLAG_POS_MASK(bit),											\
		.flag = (#name),													\
		.ival = 0															\
	}																		\

# define	U_SIMPLE_FLAG_BIT(argtype, bit, letter)							\
	FLAG_##letter = bit														\

# define	U_DOUBLE_FLAG_BIT(argtype, bit, name)							\
	FLAG_##name = bit														\

# define	U_FLAG_SIMPLE(argtype, bit, letter)								\
	U_SIMPLE, argtype, bit, letter											\

# define	U_FLAG_DOUBLE(argtype, bit, name)								\
	U_DOUBLE, argtype, bit, name											\

# define	U_FLAG_BIT_GEN(spec)											\
	U_FLAG_BIT_DISPATCH(spec)												\

# define	U_FLAG_DEF_GEN(spec)											\
	U_FLAG_DEF_DISPATCH(spec)												\

# define	U_FLAG_BIT_DISPATCH(type, ...)									\
	type##_FLAG_BIT(__VA_ARGS__)											\

# define	U_FLAG_DEF_DISPATCH(type, ...)									\
	type##_FLAG_DEF(__VA_ARGS__)											\

# define	U_FLAGS_GEN(name, list)			\
typedef enum								\
{											\
	list(U_FLAG_BIT_GEN)					\
}	name##_u_bitflags;						\

#endif // _UFLAGS_H
