/**
 *	celf_utils.h | CELF tools.
 * -------------------------------------------------------------------------- */

#ifndef _CELF_UTILS_H
# define _CELF_UTILS_H

# include <unistd.h>
# include <stdint.h>
# include <stdlib.h>
# include <fcntl.h>
# include <sys/stat.h>
# include <sys/mman.h>

# include <u_printf.h>

typedef struct	_elf_s			ELF;
typedef struct	_celf_ctx_s		CELF;

/* UTILS MACROS ------------------------------------------------------------- */

# define	CELF_CTX		_celf_ctx
# define	CELF_FILE		CELF_CTX.file
# define	ELF_RAW			CELF_FILE.raw
# define	ELF_SIZE		CELF_FILE.size
# define	ELF_FILENAME	CELF_FILE.filename

# define	HOST_IS_LE 0
# if defined(__BYTE_ORDER__) && __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
#  undef	HOST_IS_LE
#  define	HOST_IS_LE 1
# endif

# if !defined(CELF_IS_LE)
#  define	CELF_IS_LE	ELF_RAW[EI_DATA] == ELF_DATA2LSB
# endif

# if defined(CELF_IS_LE) && defined(HOST_IS_LE)
#  define		READ_FIELD_INNER(field, size)								\
    ((HOST_IS_LE != (CELF_IS_LE)) ? (										\
        (size) == 2 ? byteswap16(field) :									\
        (size) == 4 ? byteswap32(field) :									\
        (size) == 8 ? byteswap64(field) : (field)							\
    ) : (field))
#  define	READ_FIELD(field)	READ_FIELD_INNER(field, sizeof(field))
# endif

# define	ELF_OFFSET(o)			(ELF_RAW + (o))
# define	ELF_PTR_CHECK(ptr)		((ptr - ELF_RAW) >= ELF_SIZE)

# define	UNUSED		__attribute__((unused))
# define	PACKED		__attribute__((packed))
# define	DESTRUCTOR	__attribute__((destructor))
# define	INLINE		static inline

# if !defined(CONCAT) || !defined(CONCAT_INNER)
#  define	CONCAT_INNER(a, b)	a##b
#  define	CONCAT(a, b)		CONCAT_INNER(a, b)
# endif

# define	CELF_API(type, name, ...)	type name(__VA_ARGS__)
# define	CELF_PREFIX					CONCAT(ELF, CELF_CLASS)
# define	CELF(X)						CONCAT(CELF_PREFIX, CONCAT(_, X))

/* CONSTANTS ---------------------------------------------------------------- */

# define	ELF_MINSIZE		sizeof(ELF32_Hdr)

/* ERROR HANDLING ----------------------------------------------------------- */

# define	_celf_fail_open		"open() call failed."
# define	_celf_fail_stat		"stat() call failed."
# define	_celf_fail_mmap		"mmap() call failed."

# define	_celf_fail_ptrchk	"invalid pointer offset."
# define	_celf_fail_zerodiv	"division by zero incoming."

# define	_celf_invalid_magic		"invalid ELF Signature."
# define	_celf_invalid_class		"invalid ELF Class."
# define	_celf_invalid_endian	"invalid ELF Endianness."
# define	_celf_invalid_version	"invalid ELF Version."
# define	_celf_invalid_ABI		"unknown ABI."

#define		CELF_ERROR(msg)													\
{																			\
	u_dprintf																\
	(																		\
		STDERR_FILENO,														\
		"CELF: error at %s:%d in %s\n > %s",								\
		__FILE__, __LINE__, __func__, msg									\
	);																		\
}																			\

# define	CELF_THROW(msg)	do												\
{																			\
	CELF_ERROR(msg)															\
	exit(1);																\
} while (0)

/* BYTESWAP SUPPORT --------------------------------------------------------- */

INLINE	uint16_t	byteswap16(uint16_t x)
{
	return (x >> 8) | (x << 8);
}

INLINE	uint32_t	byteswap32(uint32_t x)
{
	return	((x & 0xFF000000U) >> 24)	|
			((x & 0x00FF0000U) >> 8)	|
			((x & 0x0000FF00U) << 8)	|
			((x & 0x000000FFU) << 24);
}

INLINE	uint64_t	byteswap64(uint64_t x)
{
    return ((x & 0xFF00000000000000ULL) >> 56) |
           ((x & 0x00FF000000000000ULL) >> 40) |
           ((x & 0x0000FF0000000000ULL) >> 24) |
           ((x & 0x000000FF00000000ULL) >> 8)  |
           ((x & 0x00000000FF000000ULL) << 8)  |
           ((x & 0x0000000000FF0000ULL) << 24) |
           ((x & 0x000000000000FF00ULL) << 40) |
           ((x & 0x00000000000000FFULL) << 56);
}

#endif // _CELF_UTILS_H
