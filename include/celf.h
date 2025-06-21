/**
 *	celf.h | ELF helper library.
 * -------------------------------------------------------------------------- */

#ifndef _CELF_H
# define _CELF_H

# include <celf_utils.h>

/**
 * @struct	ELF_Ident.
 *
 * @brief		16 bytes ELF File identifier.
 *
 *		e_magic			0x7F 'E' 'L' 'F'
 *		e_class			File class			(32 or 64 bit)
 *		e_endianness	File endianness		(from offset 0x10)
 *		e_version		ELF version			(should always be 1)
 *		e_ABI			OS ABI
 *		e_ABIversion	ABI version spec
 *		e_padding		empty space			(should be 0 filled)
 */
typedef union
{
	struct
	{
		uint8_t	e_magic[4];
		uint8_t	e_class;
		uint8_t	e_endianness;
		uint8_t	e_version;
		uint8_t	e_ABI;
		uint8_t	e_ABIversion;
		uint8_t	e_padding[7];
	};
	uint8_t	e_ident[16];
}	ELF_Ident;

# define	ELF_MAG0	'\x7f'
# define	ELF_MAG1	'E'
# define	ELF_MAG2	'L'
# define	ELF_MAG3	'F'

# define	ELF_MAGIC	0x464C457F

/**
 * @struct	ELF64_Hdr
 *
 * @brief		64-bit ELF Header.
 *
 *		e_ident			Identification bytes
 *		e_type			File type
 *		e_machine		Instruction set architecture
 *		e_version		ELF version	(should always be 1)
 *		e_entry			Program entrypoint address
 *		e_phoff			Program header table offset
 *		e_shoff			Section header table offset
 *		e_flags			Architecture-specific
 *		e_hsize			ELF header size
 *		e_phsize		Program header table entry size
 *		e_phnum			Program header table entry count
 *		e_shsize		Section header table entry size
 *		e_shnum			Section header table entry count
 *		e_shstridx		Section header index for section names
 */
typedef struct
{
	ELF_Ident	e_ident;
	uint16_t	e_type;
	uint16_t	e_machine;
	uint32_t	e_version;
	uint64_t	e_entry;
	uint64_t	e_phoff;
	uint64_t	e_shoff;
	uint32_t	e_flags;
	uint16_t	e_hsize;
	uint16_t	e_phsize;
	uint16_t	e_phnum;
	uint16_t	e_shsize;
	uint16_t	e_shnum;
	uint16_t	e_shstridx;
}	PACKED ELF64_Hdr;

/**
 * @struct	ELF32_Hdr
 *
 * @brief		32-bit ELF Header.
 *
 *		e_ident			Identification bytes
 *		e_type			File type
 *		e_machine		Instruction set architecture
 *		e_version		ELF version	(should always be 1)
 *		e_entry			Program entrypoint address
 *		e_phoff			Program header table offset
 *		e_shoff			Section header table offset
 *		e_flags			Architecture-specific
 *		e_hsize			ELF header size
 *		e_phsize		Program header table entry size
 *		e_phnum			Program header table entry count
 *		e_shsize		Section header table entry size
 *		e_shnum			Section header table entry count
 *		e_shstridx		Section header index for section names
 */
typedef struct
{
	ELF_Ident	e_ident;
	uint16_t	e_type;
	uint16_t	e_machine;
	uint32_t	e_version;
	uint32_t	e_entry;
	uint32_t	e_phoff;
	uint32_t	e_shoff;
	uint32_t	e_flags;
	uint16_t	e_hsize;
	uint16_t	e_phsize;
	uint16_t	e_phnum;
	uint16_t	e_shsize;
	uint16_t	e_shnum;
	uint16_t	e_shstridx;
}	PACKED ELF32_Hdr;

typedef		uint8_t	*ELF_File;

/* PROGRAM HEADER ----------------------------------------------------------- */

/**
 * @struct	ELF64_Phdr
 *
 * @brief		64-bit Program Header.
 *
 *		p_type			Segment type
 *		p_flags			Segment flags
 *		p_offset		Offset into the file
 *		p_vaddr			Virtual address of the segment into memory
 *		p_paddr			Physical address of the segment
 *		p_filesize		Size of the segment in file
 *		p_memsize		Size of the segment in memory
 *		p_align			Alignment for p_vaddr
 */
typedef struct
{
	uint32_t	p_type;
	uint32_t	p_flags;
	uint64_t	p_offset;
	uint64_t	p_vaddr;
	uint64_t	p_paddr;
	uint64_t	p_filesize;
	uint64_t	p_memsize;
	uint64_t	p_align;
}	PACKED ELF64_Phdr;

/**
 * @struct	ELF32_Phdr
 *
 * @brief		32-bit Program Header.
 *
 *		p_type			Segment type
 *		p_offset		Offset into the file
 *		p_vaddr			Virtual address of the segment into memory
 *		p_paddr			Physical address of the segment
 *		p_filesize		Size of the segment in file
 *		p_memsize		Size of the segment in memory
 *		p_flags			Segment flags
 *		p_align			Alignment for p_vaddr
 */
typedef struct
{
	uint32_t	p_type;
	uint32_t	p_offset;
	uint32_t	p_vaddr;
	uint32_t	p_paddr;
	uint32_t	p_filesize;
	uint32_t	p_memsize;
	uint32_t	p_flags;
	uint32_t	p_align;
}	PACKED ELF32_Phdr;

/* SECTION HEADER ----------------------------------------------------------- */

/**
 * @struct	ELF64_Shdr
 *
 * @brief		64-bit Section Header.
 *
 *		sh_name			Section name. SHT_STRTAB index
 *		sh_type			Section type, see ELF_SectionType
 *		sh_flags		Section flags
 *		sh_addr			Section virtual address
 *		sh_offset		Section offset in file
 *		sh_size			Section size
 *		sh_link			Link to another section
 *		sh_info			Additional information
 *		sh_align		Section alignment
 *		sh_entsize		Entry size if a table is held
 */
typedef struct
{
	uint32_t	sh_name;
	uint32_t	sh_type;
	uint64_t	sh_flags;
	uint64_t	sh_addr;
	uint64_t	sh_offset;
	uint64_t	sh_size;
	uint32_t	sh_link;
	uint32_t	sh_info;
	uint64_t	sh_align;
	uint64_t	sh_entsize;
}	PACKED ELF64_Shdr;

/**
 * @struct	ELF32_Shdr
 *
 * @brief		32-bit Section Header.
 *
 *		sh_name			Section name. SHT_STRTAB index
 *		sh_type			Section type, see ELF_SectionType
 *		sh_flags		Section flags
 *		sh_addr			Section virtual address
 *		sh_offset		Section offset in file
 *		sh_size			Section size
 *		sh_link			Link to another section
 *		sh_info			Additional information
 *		sh_align		Section alignment
 *		sh_entsize		Entry size if a table is held
 */
typedef struct
{
	uint32_t	sh_name;
	uint32_t	sh_type;
	uint32_t	sh_flags;
	uint32_t	sh_addr;
	uint32_t	sh_offset;
	uint32_t	sh_size;
	uint32_t	sh_link;
	uint32_t	sh_info;
	uint32_t	sh_addralign;
	uint32_t	sh_entsize;
}	PACKED ELF32_Shdr;

/* SYMBOLS ------------------------------------------------------------------ */

/**
 * @struct	ELF64_Sym
 *
 * @brief		64-bit ELF Symbol.
 *
 *		sh_name			Symbol name. SHT_STRTAB index.
 *		st_info			Symbol type and biding. see macros and enums
 *		st_other		Symbol visibility
 *		st_shidx		Section index
 *		st_value		Symbol value
 *		st_size			Symbol size
 */
typedef struct
{
	uint32_t	st_name;
	uint8_t		st_info;
	uint8_t		st_other;
	uint16_t	st_shidx;
	uint64_t	st_value;
	uint64_t	st_size;
}	PACKED ELF64_Sym;

# define		ELF64_ST_BIND(info)			(((uint8_t)(info)) >> 4)
# define		ELF64_ST_TYPE(info)			((info) & 0xF)
# define		ELF64_ST_INFO(bind, type)	(((bind) << 4) | ((type) & 0xF)))

/**
 * @struct	ELF32_Sym
 *
 * @brief		32-bit ELF Symbol.
 *
 *		sh_name			Symbol name. SHT_STRTAB index
 *		st_value		Symbol value
 *		st_size			Symbol size
 *		st_info			Symbol type and biding. see macros and enums
 *		st_other		Symbol visibility
 *		st_shidx		Section index
 */
typedef struct
{
	uint32_t	st_name;
	uint32_t	st_value;
	uint32_t	st_size;
	uint8_t		st_info;
	uint8_t		st_other;
	uint16_t	st_shidx;
}	PACKED ELF32_Sym;

# define		ELF32_ST_BIND(info)			ELF64_ST_BIND(info)
# define		ELF32_ST_TYPE(info)			ELF64_ST_TYPE(info)
# define		ELF32_ST_INFO(bind, type)	ELF64_ST_INFO(bind, type)

# define		ST_BIND(info)				ELF32_ST_BIND(info)
# define		ST_TYPE(info)				ELF32_ST_TYPE(info)

/* DYNAMIC ------------------------------------------------------------------ */

/**
 * @struct	ELF64_Dyn
 *
 * @brief		64-bit Dynamic symbol
 *
 *		d_tag			Contains a symbol-specific tag (see ELF_DynamicTag)
 *		d_un			Contains additional data either as int or addr
 *			d_val			Integer value.
 *			d_ptr			Adress value.
 *
 */
typedef struct
{
	int64_t	d_tag;
	union
	{
		uint64_t	d_val;
		uint64_t	d_ptr;
	}	d_un;
}	PACKED ELF64_Dyn;

/**
 * @struct	ELF32_Dyn
 *
 * @brief		32-bit Dynamic symbol
 *
 *		d_tag			Contains a symbol-specific tag (see ELF_DynamicTag)
 *		d_un			Contains additional data either as int or addr
 *			d_val			Integer value.
 *			d_ptr			Adress value.
 *
 */
typedef struct
{
	int32_t	d_tag;
	union
	{
		uint32_t	d_val;
		uint32_t	d_ptr;
	}	d_un;
}	PACKED ELF32_Dyn;

/* DYNAMIC ------------------------------------------------------------------ */

/**
 * @struct	ELF64_Rel
 *
 * @brief		64-bit Relocation
 *
 *			r_offset		where the relocation takes place (ET_REL != ET_EXEC)
 *			r_info			Index of symbol table and relocation type
 */
typedef struct
{
	uint64_t	r_offset;
	uint64_t	r_info;
}	PACKED ELF64_Rel;

/**
 * @struct	ELF32_Rel
 *
 * @brief		32-bit Relocation
 *
 *			r_offset		where the relocation takes place (ET_REL != ET_EXEC)
 *			r_info			Index of symbol table and relocation type
 */
typedef struct
{
	uint32_t	r_offset;
	uint32_t	r_info;
}	PACKED ELF32_Rel;

/**
 * @struct	ELF64_Rel
 *
 * @brief		64-bit Relocation with addend
 *
 *			r_offset		Location of relocation (ET_REL != ET_EXEC)
 *			r_info			Index of symbol table and relocation type
 *			r_addend		Constant value for address calculation
 */
typedef struct
{
	uint64_t	r_offset;
	uint64_t	r_info;
	int64_t		r_addend;
}	PACKED ELF64_Rela;

/**
 * @struct	ELF32_Rel
 *
 * @brief		32-bit Relocation with addend
 *
 *			r_offset		Location of relocation (ET_REL != ET_EXEC)
 *			r_info			Index of symbol table and relocation type
 *			r_addend		Constant value for address calculation
 */
typedef struct
{
	uint32_t	r_offset;
	uint32_t	r_info;
	int32_t		r_addend;
}	PACKED ELF32_Rela;

/* -------------------------------------------------------------------------- */

extern	CELF _celf_ctx;

# include <celf_enums.h>
# include <celf_context.h>

CELF_API(int, ELF_open, const char *filename);
CELF_API(DESTRUCTOR void, ELF_close);

CELF_API(int, ELF_check);
CELF_API(int, ELF_is64bit);
CELF_API(int, ELF_is32bit);

#define _CELF_WRAPPER(														\
	name, ret, type32, type64, wrap_args, args32, args64, call32, call64)	\
																			\
    type32 CONCAT(ELF32_, name) args32;										\
    type64 CONCAT(ELF64_, name) args64;										\
																			\
    INLINE ret CONCAT(ELF_, name) wrap_args									\
    {																		\
        if (ELF_is32bit())													\
            return CONCAT(ELF32_, name) call32;								\
        return CONCAT(ELF64_, name) call64;									\
    }

#define CELF_WRAPPER_PTR_VOID(name, type)									\
    _CELF_WRAPPER(															\
        name, void *,														\
        CONCAT(ELF32_, type) *, CONCAT(ELF64_, type) *,						\
        (void), (void), (void),												\
        (), ()																\
    )

#define	CELF_WRAPPER_OTHER_VOID(name, type)									\
	_CELF_WRAPPER(															\
		name, type,															\
		type, type,															\
		(void), (void), (void),												\
		(), ()																\
	)

#define CELF_WRAPPER_PTR_SAME(name, type, args, call)						\
    _CELF_WRAPPER(															\
        name, void *,														\
        CONCAT(ELF32_, type) *, CONCAT(ELF64_, type) *,						\
        args, args, args,													\
        call, call															\
    )

#define CELF_WRAPPER_PTR_DIFF(name, ret, type)								\
    _CELF_WRAPPER(															\
        name, void *,														\
        CONCAT(ELF32_, ret) *, CONCAT(ELF64_, ret) *,						\
        (void *ptr),														\
        (CONCAT(ELF32_, type) *ptr), (CONCAT(ELF64_, type) *ptr),			\
        ((CONCAT(ELF32_, type) *)ptr), ((CONCAT(ELF64_, type) *)ptr)		\
    )

#define CELF_WRAPPER_GENERIC(												\
	name, ret, type32, type64, wrap_args, args32, args64, call32, call64)	\
																			\
    _CELF_WRAPPER(															\
        name, ret,															\
        type32, type64,														\
        wrap_args, args32, args64,											\
        call32, call64														\
    )

/**
 *	@brief	ELF_header_get()
 * -------------------------------------------------------------------------- */
CELF_WRAPPER_PTR_VOID(
	header_get,
	Hdr
)

/**
 *	@brief	ELF_sheaders_get()
 * -------------------------------------------------------------------------- */
CELF_WRAPPER_PTR_VOID(
	sheaders_get,
	Shdr
)

CELF_WRAPPER_OTHER_VOID(
	sheaders_size,
	uint16_t
)

/**
 *	@brief	ELF_pheaders_get()
 * -------------------------------------------------------------------------- */
CELF_WRAPPER_PTR_VOID(
	pheaders_get,
	Phdr
)

CELF_WRAPPER_OTHER_VOID(
	pheaders_size,
	uint16_t
)

/**
 *	@brief	ELF_section_content(void *section)
 * -------------------------------------------------------------------------- */
CELF_WRAPPER_GENERIC(
	section_content,
	uint8_t *,
	uint8_t *,
	uint8_t *,
	(void *section),
	(CONCAT(ELF32_, Shdr) *section),
	(CONCAT(ELF64_, Shdr) *section),
	(section),
	(section)
)

/**
 *	@brief	ELF_section_content(void *section)
 * -------------------------------------------------------------------------- */
CELF_WRAPPER_GENERIC(
	section_name,
	const char *,
	const char *,
	const char *,
	(void *section),
	(CONCAT(ELF32_, Shdr) *section),
	(CONCAT(ELF64_, Shdr) *section),
	(section),
	(section)
)

/**
 *	@brief	ELF_shstrtab_get()
 * -------------------------------------------------------------------------- */
CELF_WRAPPER_OTHER_VOID(
	shstrtab_get,
	char *
)

/**
 *	@brief	ELF_sheader_get_by_name(const char *target)
 * -------------------------------------------------------------------------- */
CELF_WRAPPER_PTR_SAME(
	sheader_get_by_name,
	Shdr,
	(const char *target),
	(target)
)

/**
 *	@brief	ELF_sheader_get_by_idx(uint16_t idx)
 * -------------------------------------------------------------------------- */
CELF_WRAPPER_PTR_SAME(
	sheader_get_by_idx,
	Shdr,
	(uint16_t idx),
	(idx)
)

/**
 *	@brief	ELF_symbols_get()
 * -------------------------------------------------------------------------- */
CELF_WRAPPER_PTR_VOID(
	symtab_get,
	Sym
)

/**
 *	@brief	ELF_symbols_get()
 * -------------------------------------------------------------------------- */
CELF_WRAPPER_OTHER_VOID(
	symtab_size,
	uint64_t
)

#endif	// _CELF_H
