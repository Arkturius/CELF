/**
 *	u_printf - unofficial printf clone
 *
 *	  contains:	u_printf
 *				u_dprintf
 *				u_vprintf
 *				u_vdprintf
 *
 *	  utils:	u_lltoa
 *				u_ulltoa
 *				u_ulxtoa
 *
 *	Currently supported:
 *		formats:	c s i d u x p
 *		prefix:		l (for i, d, u and x) 0 (N) -
 * -------------------------------------------------------------------------- */

#ifndef _UPRINTF_H
# define _UPRINTF_H

# include <unistd.h>
# include <stdint.h>
# include <stdarg.h>

/**
 *	uprintf	- writes formatted string to SDTOUT
 */
__attribute__((format(printf, 1, 2)))
int	u_printf(const char *fmt, ...);

/**
 *	udprintf	- writes formatted string to fd
 */
__attribute__((format(printf, 2, 3)))
int	u_dprintf(int fd, const char *fmt, ...);

/**
 *	uvprintf	- writes formatted string to STDIN, using va_list
 */
__attribute__((format(printf, 1, 0)))
int	u_vprintf(const char *fmt, va_list ap);

/**
 *	uvdprintf	- writes formatted string to fd, using va_list
 */
__attribute__((format(printf, 2, 0)))
int	u_vdprintf(int fd, const char *fmt, va_list ap);


/**
 * libc clones.
 */

uint32_t	u_strlen(const char *src);

char		*u_strcat(char *dst, const char *src);

int			u_strcmp(const char *s1, const char *s2);

int			u_strncmp(const char *s1, const char *s2, uint32_t n);

void		*u_memset(void *dst, int c, uint32_t n);

void		*u_memcpy(void *dst, void *src, uint32_t n);

#endif // _UPRINTF_
