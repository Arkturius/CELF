/**
 *	stringify.h | generic stringify function generator
 * -------------------------------------------------------------------------- */

#if !defined(STY_UTILS_FLAG)

# define STY_UTILS_FLAG

# define	STY_CONCAT_INNER(a, b)	a##b
# define	STY_CONCAT(a, b)		STY_CONCAT_INNER(a, b)
# define	STY_FUNC_NAME(type)		STY_CONCAT(type, _stringify)

# define	STY_NULL_STRUCT		((STRUCT_TYPE *)(0))
# define	STY_FIELD(name)		STY_NULL_STRUCT->name
# define	STY_OFFSET(name)	((unsigned long int)&(STY_FIELD(name)))

# define	STY_ARRAY_SIZE(a)	(sizeof(a) / sizeof(a[0]))
# define	STY_FOREACH(v, a)	for (v = &a[0]; v < a + STY_ARRAY_SIZE(a); ++v)

#endif // STY_UTILS_FLAG

/* ENUM STRINGIFICATION ------------------------------------------------------ */

#if defined(ENUM_TYPE)

# if !defined(ENUM_LIST)
#  error "ENUM_LIST must be defined."
# endif

# define STY_ENUM_NAME(type)	STY_CONCAT(type, _e)

typedef enum STY_ENUM_NAME(ENUM_TYPE) ENUM_TYPE;

enum STY_ENUM_NAME(ENUM_TYPE)
{
	#define	STY_ENUM_VALS(k, v)	k = v,
	ENUM_LIST(STY_ENUM_VALS)
	#undef STY_ENUM_VALS
};

const char	*STY_FUNC_NAME(ENUM_TYPE)(ENUM_TYPE ENUM_VAR)
{
	#ifdef ENUM_SPEC
	# define STY_ENUM_IF(cond, v)	if (cond) { return ((v)); }
	ENUM_SPEC(STY_ENUM_IF)
	# undef STY_ENUM_IF
	#endif

	static struct pair { ENUM_TYPE k; const char *v; } pairs[] = 
	{
		#define STY_ENUM_STRINGS(k, v) {(k), (#k)},
		ENUM_LIST(STY_ENUM_STRINGS)
		#undef	STY_ENUM_STRINGS
	};
	
	struct pair	*pair;

	STY_FOREACH(pair, pairs)
	{
		if (pair->k == ENUM_VAR)
			return (pair->v);
	}
	return ("UNKNOWN");
}

# undef	ENUM_TYPE
# undef	ENUM_LIST
# undef	ENUM_SPEC

#endif // ENUM_TYPE

/* STRUCT STRINGIFICATION ----------------------------------------------------- */

#if defined(STRUCT_TYPE)

#include <unistd.h>

# if !defined (STRUCT_TYPE) || !defined(STRUCT_FIELDS)
#  error "STRUCT_TYPE and STRUCT_FIELDS must be defined."
# endif

# define	STY_STRUCT_NAME(type)	STY_CONCAT(type, _s)

typedef struct STY_STRUCT_NAME(STRUCT_TYPE) STRUCT_TYPE;

struct STY_STRUCT_NAME(STRUCT_TYPE)
{
	#define STRUCT_FIELD(type, name)	type name;
	STRUCT_FIELDS(STRUCT_FIELD)
	#undef STRUCT_FIELD
};

const char	*STY_FUNC_NAME(STRUCT_TYPE)(STRUCT_TYPE STRUCT_VAR)
{
	static struct field { const char *name; int size; int off; } fields[] = 
	{
		#define	STRUCT_FIELD(type, name)									\
		{#name, sizeof(STY_FIELD(name)), STY_OFFSET(name)},
		STRUCT_FIELDS(STRUCT_FIELD)
		#undef STRUCT_FIELD
	};

	(void)	STRUCT_VAR;
	struct field *field;

	STY_FOREACH(field, fields)
	{
	}
	return ("UNKNOWN");
}

# undef STRUCT_TYPE
# undef STRUCT_FIELDS

/* -------------------------------------------------------------------------- */

#endif // STRUCT_TYPE
