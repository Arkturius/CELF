/**
 *	stringify.h | generic stringify function generator
 ******************************************************************************/

#if !defined (ENUM_TYPE) || !defined(ENUM_LIST)
# error "ENUM_TYPE and ENUM_LIST must be defined."
#endif

#if defined(STY_CONCAT_INNER) || defined(STY_CONCAT) || defined(STY_FUNC_NAME)
# error "conflicting macros: STY_CONCAT or STY_CONCAT_INNER or STY_FUNC_NAME"
#endif

#define	STY_CONCAT_INNER(a, b)	a##b
#define	STY_CONCAT(a, b)		STY_CONCAT_INNER(a, b)
#define	STY_FUNC_NAME(type)		STY_CONCAT(type, _stringify)

/**
 *		-	Generate an X-macro list for your enum
 *
 *		#define		e_exampleList(EXPAND)	\
 *			EXPAND(example0, 0)				\
 *			EXPAND(example1, 1)				\
 *			EXPAND(example2, 2)				\
 *			EXPAND(example3, 3)				\
 *
 *		-	define ENUM_TYPE and ENUM_LIST then include enumToString.h
 *
 *		#define		ENUM_TYPE	e_example
 *		#define		ENUM_LIST	e_exampleList
 *		#include	"path/to/stringify.h"
 *
 *		-	You can override some of the results with special conditions.
 *			Using ENUM_VAR as placeholder and specifying a conditionnal,
 *			followed by the string that should be returned
 *
 *		#define		e_exampleSpec(EXPAND)					\
 *			EXPAND((ENUM_VAR > 2), "exampleMoreThanTwo")	\
 *			EXPAND((ENUM_VAR < 0), "exampleLessThanTwo")	\
 *
 *		#define		ENUM_SPEC	e_exampleSpec
 *
 *      -	It will generate this function, returning the LITTERAL string
 *			for ENUM_TYPE ENUM_VAR. handling ENUM_SPEC as list of exceptions
 *
 *		const char	*ENUM_TYPE_stringify(ENUM_TYPE e);
 */

#define STY_ENUM_NAME(type)		STY_CONCAT(type, _e)

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

	static const struct	{ ENUM_TYPE k; const char *v; } map[] = 
	{
		#define STY_ENUM_STRINGS(k, v) {(k), (#k)},
		ENUM_LIST(STY_ENUM_STRINGS)
		#undef	STY_ENUM_STRINGS
	};
	
	for (unsigned int i = 0; i < sizeof(map) / sizeof(map[0]); ++i)
		if (map[i].k == ENUM_VAR) { return (map[i].v); }
	return ("UNKNOWN");
}

#undef	STY_CONCAT_INNER
#undef	STY_CONCAT
#undef	STY_FUNC_NAME

#undef	ENUM_TYPE
#undef	ENUM_LIST
#undef	ENUM_SPEC
