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

/*	read-write lock types*/
enum
{
	PTHREAD_RWLOCK_PREFER_READER_NP,
	PTHREAD_RWLOCK_PREFER_WRITER_NP,		/*	dummy*/
	PTHREAD_RWLOCK_PREFER_WRITER_NONRECURSIVE_NP,
	PTHREAD_RWLOCK_DEFAULT_NP = PTHREAD_RWLOCK_PREFER_READER_NP
};

typedef struct pthread_rwlockattr_s
{
	int					_kind;			/*	lock kind*/
	int					_rsv0;			/*	never use*/
} pthread_rwlockattr_t;

typedef struct pthread_rwlock_s
{
	int				_lock;				/*	spinlock*/
	unsigned int	_nr_readers;		/*	number of holding for rd*/
	unsigned int	_rd_futex;
	unsigned int	_wr_futex;
	unsigned int	_nr_readers_queued;	/*	number of waiting for rd*/
	unsigned int	_nr_writers_queued;	/*	number of waiting for wr*/
	int				_writer;			/*	identity of holding wr*/
} pthread_rwlock_t;

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

LIBLWPTW_API
int pthread_mutex_lock(pthread_mutex_t * mutex);

LIBLWPTW_API
int pthread_mutex_unlock(pthread_mutex_t * mutex);

LIBLWPTW_API
int pthread_rwlock_rdlock(pthread_rwlock_t * rwlock);

LIBLWPTW_API
int pthread_rwlock_wrlock(pthread_rwlock_t * rwlock);

LIBLWPTW_API
int pthread_rwlock_unlock(pthread_rwlock_t * rwlock);

LIBLWPTW_API int
pthread_rwlockattr_init(pthread_rwlockattr_t * attr);

LIBLWPTW_API int
pthread_rwlockattr_destroy(pthread_rwlockattr_t * attr);

LIBLWPTW_API
int pthread_rwlockattr_getkind_np(
	const pthread_rwlockattr_t * attr, int * pref);

LIBLWPTW_API
int pthread_rwlockattr_setkind_np(
	pthread_rwlockattr_t * attr, int pref);

#ifdef __cplusplus
}
#endif
#endif	/*	__LIBLWPTW_PTHREAD_H__*/
