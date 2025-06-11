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
 *		prefix:		l (for i, d, u and x) 0
 */

#ifndef UPRINTF_H
# define UPRINTF_H

# include <unistd.h>
# include <stdint.h>
# include <stdarg.h>

/**
 *	uprintf	- writes formatted string to SDTOUT
 */
int	u_printf(const char *fmt, ...);

/**
 *	udprintf	- writes formatted string to fd
 */
int	u_dprintf(int fd, const char *fmt, ...);

/**
 *	uvprintf	- writes formatted string to STDIN, using va_list
 */
int	u_vprintf(const char *fmt, va_list ap);

/**
 *	uvdprintf	- writes formatted string to fd, using va_list
 */
int	u_vdprintf(int fd, const char *fmt, va_list ap);

#endif
