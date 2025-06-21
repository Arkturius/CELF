/**
 *	u_data.h - common data structures.
 * -------------------------------------------------------------------------- */

#ifndef _UDATA_
# define _UDATA_

# include <unistd.h>
# include <stdlib.h>

# include <u_printf.h>

/**
 *	linked lists.
 * -------------------------------------------------------------------------- */

# define	U_LIST(type)			U_CONCAT(type, _list)
# define	U_LIST_S(type)			U_CONCAT(U_LIST(type), _s)
# define	U_LIST_FNAME(type, n)	U_CONCAT(U_LIST(type), n)

# define	U_LIST_DECL(type)												\
																			\
typedef struct U_LIST_S(type)												\
{																			\
	struct U_LIST_S(type)	*next;											\
	type					data;											\
}	U_LIST(type)															\

# define	U_LIST_INOUT(type)												\
																			\
	U_LIST_DECL_NEW(type)													\
	U_LIST_DECL_LEN(type)													\
	U_LIST_DECL_PUSH(type)													\
	U_LIST_DECL_PUSH_RAW(type)												\
	U_LIST_DECL_POP(type)													\
	U_LIST_DECL_POP_RAW(type)												\

# define	U_LIST_DECL_NEW(type)											\
																			\
	UNUSED static inline U_LIST(type) *										\
	U_LIST_FNAME(type, _new)(type data)										\
	{																		\
		U_LIST(type)	*new = malloc(sizeof(U_LIST(type)));				\
																			\
		if (new)															\
			new->data = data;												\
		return (new);														\
	}

# define	U_LIST_DECL_LEN(type)											\
																			\
	UNUSED static inline uint32_t											\
	U_LIST_FNAME(type, _len)(U_LIST(type) *list)							\
	{																		\
		U_LIST(type)	*tmp = list;										\
		uint32_t		count = 0;											\
																			\
		while (tmp)															\
		{																	\
			tmp = tmp->next;												\
			count++;														\
		}																	\
		return (count);														\
	}

# define	U_LIST_DECL_PUSH(type)											\
																			\
	UNUSED static inline U_LIST(type) *										\
	U_LIST_FNAME(type, _push)(U_LIST(type) **list, U_LIST(type) *new)		\
	{																		\
		if (new)															\
		{																	\
			new->next = *list;												\
			*list = new;													\
		}																	\
		return (new);														\
	}

# define	U_LIST_DECL_PUSH_RAW(type)										\
																			\
	UNUSED static inline U_LIST(type) *										\
	U_LIST_FNAME(type, _pushraw)(U_LIST(type) **list, type data)			\
	{																		\
		U_LIST(type)	*new = U_LIST_FNAME(type, _new)(data);				\
																			\
		return (U_LIST_FNAME(type, _push)(list, new));						\
	}

# define	U_LIST_DECL_POP(type)											\
																			\
	UNUSED static inline U_LIST(type) *										\
	U_LIST_FNAME(type, _pop)(U_LIST(type) **list)							\
	{																		\
		U_LIST(type)	*pop = *list;										\
																			\
		if (pop)															\
			*list = pop->next;												\
		return (pop);														\
	}																		\

# define	U_LIST_DECL_POP_RAW(type)										\
																			\
	UNUSED static inline type												\
	U_LIST_FNAME(type, _popdata)(U_LIST(type) **list)						\
	{																		\
		U_LIST(type)	*pop = U_LIST_FNAME(type, _pop)(list);				\
		type			pdata = {0};										\
																			\
		if (pop)															\
		{																	\
			pdata = pop->data;												\
			free(pop);														\
		}																	\
		return (pdata);														\
	}																		\

#endif // _UDATA_H
