/**
 *	u_printf - internal functions
 */

#include <unistd.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdint.h>

/* UTILS **********************************************************************/

uint32_t	u_strlen(const char *src)
{
	const char	*tmp;

	if (!src)
		return (0);
	tmp = src;
	while (*tmp)
		tmp++;
	return (tmp - src);
}

char	*u_strcat(char *dst, const char *src)
{
	char	*tmp = dst;

	while (*src)
		*dst++ = (char)*src++;
	*dst = 0;
	return (tmp);
}

void	*u_memset(void *dst, int c, uint32_t n)
{
	uint8_t	*uchar;

	if (dst)
	{
		uchar = dst;
		while (n--)
			*uchar++ = c;
	}
	return (dst);
}

void	*u_memcpy(void *dst, void *src, uint32_t n)
{
	uint8_t	*udst;
	uint8_t	*usrc;

	if (dst && src)
	{
		udst = dst;
		usrc = src;
		while (n--)
			*udst++ = *usrc++;
	}
	return (dst);
}

int	u_strcmp(const char *s1, const char *s2)
{
	while (*s1 && *s1 == *s2)
	{
		s1++;
		s2++;
	}
	return (*s1 - *s2);
}

static const char	*u_lltoa(long l)
{
	static char	buff[22] = {0};
	char		*cursor;

	u_memset(buff, 0, sizeof(buff));
	cursor = &buff[20];
	if (!l)
	{
		*cursor = '0';
		return (cursor);
	}
	while (l)
	{
		char	c = l % 10;
		if (l < 0)
			c = -c;
		*cursor-- = c + 48;
		if (l > -10 && l < 0)
			*cursor-- = '-';
		l /= 10;
	}
	cursor++;
	return (cursor);
}

static const char	*u_ulltoa(unsigned long l)
{
	static char	buff[21] = {0};
	char		*cursor;

	u_memset(buff, 0, sizeof(buff));
	cursor = &buff[19];
	if (!l)
	{
		*cursor = '0';
		return (cursor);
	}
	while (l)
	{
		char	c = l % 10;
		*cursor-- = c + 48;
		l /= 10;
	}
	cursor++;
	return (cursor);
}

const char	*u_ulxtoa(unsigned long l, int upper)
{
	static char	buff[17] = {0};
	static char	*hex = "0123456789abcdef0123456789ABCDEF";
	char		*cursor;

	u_memset(buff, 0, sizeof(buff));
	cursor = &buff[15];
	if (!l)
	{
		*cursor = '0';
		return (cursor);
	}
	while (l)
	{
		char	c = l & 0xF;
		if (upper)
			c |= 0x10;
		*cursor-- = hex[(int)c];
		l >>= 4;
	}
	cursor++;
	return (cursor);
}

static const char	*u_ptrtoa(unsigned long l)
{
	static char	buff[19] = {0};

	if (l != 0)
	{
		u_strcat(buff, "0x");
		u_strcat(buff + 2, u_ulxtoa(l, 0));
	}
	else
		u_strcat(buff, "(nil)");
	return (buff);
}

/* CONTEXT ********************************************************************/

typedef struct	u_printf_ctx
{
	uint32_t	fd;
	uint32_t	total;
	uint32_t	len;
	char		*out;
}	printf_ctx;

printf_ctx	__upf_ctx = {0};

static inline void	__upf_ctx_init(void)
{
	if (!__upf_ctx.out)
		__upf_ctx.out = malloc(1024 * sizeof(char));
	if (!__upf_ctx.out)
		exit(1);
}

__attribute__((destructor))
static void	__upf_ctx_destroy(void)
{
	free(__upf_ctx.out);
}

static inline void	__upf_flush(void)
{
	int	w = write(__upf_ctx.fd, __upf_ctx.out, __upf_ctx.len);
	
	if (w < 0)
		exit(1);
	__upf_ctx.total += w;
	__upf_ctx.len = 0;
}

static inline void	__upf_ctx_cat(const char *src, uint32_t len)
{
	if (__upf_ctx.len + len >= 1024)
		__upf_flush();

	while (len--)
		__upf_ctx.out[__upf_ctx.len++] = *src++;
	__upf_ctx.out[__upf_ctx.len] = 0;
}

/* PRINTER ********************************************************************/

enum	uprintf_flags
{
	U_LONG	= 1 << 0,
	U_ZERO	= 1 << 2,
};

static inline uint32_t	u_switch_len(const char **fmt_ptr)
{
	const char	*fmt = *fmt_ptr;
	uint32_t	z = 0;

	while (*fmt && *fmt >= '0' && *fmt <= '9')
	{
		z = z * 10 + (*fmt - 48);
		fmt++;
	}
	*fmt_ptr = fmt;
	return (z);
}

static inline uint32_t	u_switch_prefix(const char **fmt_ptr, uint32_t *pad_ptr)
{
	const char	*fmt = *fmt_ptr;
	uint32_t	flags = 0;

	fmt++;
	if (*fmt == '0')
	{
		flags |= U_ZERO;
		fmt++;
		*pad_ptr = u_switch_len(&fmt);
	}
	if (*fmt == 'l')
	{
		flags |= U_LONG;
		fmt++;
	}
	*fmt_ptr = fmt;
	return (flags);
}

static inline void	u_switch_flags(const char **fmt_ptr, va_list ap)
{
	const char	*fmt = *fmt_ptr;
	const char	*to_add;
	uint32_t	flags = 0;
	uint32_t	pad = 0;
	uint32_t	len = 0;
	char		c;

	flags = u_switch_prefix(&fmt, &pad);
	switch (*fmt)
	{
		case 'c':
			c = va_arg(ap, int);
			to_add = &c;
			len = 1;
			goto end;
		case 's':
			to_add = va_arg(ap, char *);
			break ;
		case 'i': 
		case 'd':
			if (flags & U_LONG)
				to_add = u_lltoa(va_arg(ap, int));
			else
				to_add = u_lltoa(va_arg(ap, long));
			break ;
		case 'u':
			if (flags & U_LONG)
				to_add = u_ulltoa(va_arg(ap, unsigned int));
			else
				to_add = u_ulltoa(va_arg(ap, unsigned long));
			break ;
		case 'x':
			to_add = u_ulxtoa(va_arg(ap, unsigned int), 0);
			break ;
		case 'X':
			to_add = u_ulxtoa(va_arg(ap, unsigned int), 1);
			break ;
		case 'p':
			to_add = u_ptrtoa(va_arg(ap, unsigned long));
			break ;
		default:
			write(STDERR_FILENO, "Unsupported format.\n", 20);
			exit(1);
			return ;
	}
	len = u_strlen(to_add);
	if (*fmt != 'p' && (flags & U_ZERO))
	{
		int	diff = pad - len - (*to_add == '-');

		c = '0';
		while (diff-- > 0)
			__upf_ctx_cat(&c, 1);
	}

end:
	__upf_ctx_cat(to_add, len);
	*fmt_ptr = fmt;
}

__attribute__((format(printf, 2, 0)))
int	u_vdprintf(int fd, const char *fmt, va_list ap)
{
	if (!fmt)
		return (-1);

	__upf_ctx_init();
	__upf_ctx.fd = fd;
	while (*fmt)
	{
		switch (*fmt)
		{
			case '%':
				u_switch_flags(&fmt, ap);
				break ;
			case '\n':
				__upf_ctx_cat(fmt, 1);
				__upf_flush();
				break ;
			default:
				__upf_ctx_cat(fmt, 1);
				break ;
		}
		fmt++;
	}
	return (__upf_ctx.total);
}

__attribute__((format(printf, 1, 0)))
int	u_vprintf(const char *fmt, va_list ap)
{
	return u_vdprintf(STDOUT_FILENO, fmt, ap);
}

__attribute__((format(printf, 1, 2)))
int	u_printf(const char *fmt, ...)
{
	int		res;
	va_list	ap;

	if (!fmt)
		return (-1);
	va_start(ap, fmt);
	res = u_vprintf(fmt, ap);
	va_end(ap);
	return (res);
}

__attribute__((format(printf, 2, 3)))
int	u_dprintf(int fd, const char *fmt, ...)
{
	int		res;
	va_list	ap;

	if (!fmt)
		return (-1);
	va_start(ap, fmt);
	res = u_vdprintf(fd, fmt, ap);
	va_end(ap);
	return (res);
}
