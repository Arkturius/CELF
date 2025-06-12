/**
 *	stringify.h | generic stringify function generator
 * -------------------------------------------------------------------------- */

#if !defined(STY_UTILS_FLAG)

# define STY_UTILS_FLAG
# include <u_printf.h>

# define	STY_CONCAT_INNER(a, b)	a##b
# define	STY_CONCAT(a, b)		STY_CONCAT_INNER(a, b)
# define	STY_FUNC_NAME(type)		STY_CONCAT(type, _stringify)

# define	STY_NULL_STRUCT		((STRUCT_TYPE *)(0))
# define	STY_FIELD(name)		STY_NULL_STRUCT->name
# define	STY_OFFSET(name)	((unsigned long)&(STY_FIELD(name)))

# define	STY_ARRAY_SIZE(a)	(sizeof(a) / sizeof(a[0]))
# define	STY_FOREACH(v, a)	for (v = &a[0]; v < a + STY_ARRAY_SIZE(a); ++v)

typedef	void	(*STY_FORMATTER)(void *data, uint8_t flags);

enum
{
	STY_FORMAT_CHAR		= 0,
	STY_FORMAT_SHORT	= 1,
	STY_FORMAT_INT		= 2,
	STY_FORMAT_LONG		= 3,

	STY_FORMAT_UNSIGNED	= 1 << 2,
	STY_FORMAT_HEX		= STY_FORMAT_UNSIGNED | (1 << 3),

	STY_FORMAT_MAX		= 0x7F,
};

void	sty_fmtchar(void *data, uint8_t flags)
{
	int8_t	c;

	(void) flags;
	c = *(int8_t *)data;
	u_printf("%c\n", c);
}

void	sty_fmtlong(void *data, uint8_t flags)
{
	uint16_t	s = *(uint16_t *) data;
	uint32_t	i = *(uint32_t *) data;
	uint64_t	l = *(uint64_t *) data;

	const char	*format =
		((flags & STY_FORMAT_HEX) == STY_FORMAT_HEX) 
		?	"0x%lx\n"
		:	(flags & STY_FORMAT_UNSIGNED)
			?	"%lu\n"
			:	"%ld\n";

	flags &= 0b11;
	if (flags == STY_FORMAT_SHORT)
		u_printf(format, s);
	if (flags == STY_FORMAT_INT)
		u_printf(format, i);
	if (flags == STY_FORMAT_LONG)
		u_printf(format, l);
}

static const STY_FORMATTER	sty_formatters[STY_FORMAT_MAX] =
{
	[STY_FORMAT_CHAR]	= sty_fmtchar,
	[STY_FORMAT_SHORT]	= sty_fmtlong,
	[STY_FORMAT_INT]	= sty_fmtlong,
	[STY_FORMAT_LONG]	= sty_fmtlong,
};

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

# if defined(STRUCT_PACKED)
#  define	STY_PACKED	__attribute__((packed))
# else
#  define	STY_PACKED	
# endif

typedef struct STY_STRUCT_NAME(STRUCT_TYPE) STRUCT_TYPE;

struct STY_STRUCT_NAME(STRUCT_TYPE)
{
	#define STRUCT_FIELD(type, name, ftype)	type name;
	STRUCT_FIELDS(STRUCT_FIELD)
	#undef STRUCT_FIELD
}	STY_PACKED;

const char	*STY_FUNC_NAME(STRUCT_TYPE)(STRUCT_TYPE *STRUCT_VAR)
{
	static struct field { const char *name; int size; int off; uint8_t ftype; } fields[] = 
	{
		#define	STRUCT_FIELD(type, name, ftype)									\
		{#name, sizeof(STY_FIELD(name)), STY_OFFSET(name), ftype},
		STRUCT_FIELDS(STRUCT_FIELD)
		#undef STRUCT_FIELD
	};

	struct field *field;

	u_printf("{\n");
	STY_FOREACH(field, fields)
	{
		uint8_t	fmt_type = field->size == 8
			?	STY_FORMAT_LONG
			:	field->size == 4
			?	STY_FORMAT_INT
			:	field->size == 2
			?	STY_FORMAT_SHORT
			:	field->size == 1
			?	STY_FORMAT_CHAR
			:	STY_FORMAT_MAX;

		if (fmt_type == STY_FORMAT_MAX)
			continue ;
		u_printf("\t%s:\t", field->name);
		sty_formatters[fmt_type]((uint8_t *)STRUCT_VAR + field->off, fmt_type | field->ftype);
	}
	u_printf("}\n");
	return ("PLACEHOLDER");
}

# undef STRUCT_TYPE
# undef STRUCT_FIELDS
# undef STRUCT_PACKED
# undef STY_PACKED

/* -------------------------------------------------------------------------- */

#endif // STRUCT_TYPE
