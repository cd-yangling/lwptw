/**
 *	pthread.h
 *
 *	Copyright (C) 2014 YangLing(yl.tienon@gmail.com)
 *
 *	Description:
 *
 *	Revision History:
 *
 *	2015-05-15 Created By YangLing
 */

#ifndef	__LIBLWPTW_PTHREAD_H__
#define	__LIBLWPTW_PTHREAD_H__

typedef struct pthread_mutex_s
{
	int					_futex;
	int					_owner;
	unsigned int		_count;

} pthread_mutex_t;

/**
 *	ETIMEDOUT hasn't been defined in errno.h
 */
#ifndef ETIMEDOUT
#  define ETIMEDOUT 10060	/* This is the value in winsock.h. */
#endif

#if defined(LIBLWPTW_EXPORTS)
#	define	LIBLWPTW_API __declspec(dllexport)
#else
#	define	LIBLWPTW_API __declspec(dllimport)
#endif

#ifdef	__cplusplus
extern	"C" {
#endif

#ifdef __cplusplus
}
#endif
#endif	/*	__LIBLWPTW_PTHREAD_H__*/
