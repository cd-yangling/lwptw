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

/*	mutex types*/
enum
{
	/* Compatibility with LinuxThreads */
	PTHREAD_MUTEX_TIMED_NP,
	PTHREAD_MUTEX_RECURSIVE_NP,
	PTHREAD_MUTEX_ERRORCHECK_NP,
	PTHREAD_MUTEX_ADAPTIVE_NP,		/*	WARNING: never implemented*/
	/* For compatibility with POSIX */
	PTHREAD_MUTEX_NORMAL = PTHREAD_MUTEX_TIMED_NP,
	PTHREAD_MUTEX_RECURSIVE = PTHREAD_MUTEX_RECURSIVE_NP,
	PTHREAD_MUTEX_ERRORCHECK = PTHREAD_MUTEX_ERRORCHECK_NP,
	PTHREAD_MUTEX_DEFAULT = PTHREAD_MUTEX_NORMAL,
	/* For compatibility with GNU */
	PTHREAD_MUTEX_FAST_NP = PTHREAD_MUTEX_TIMED_NP
};

#define	PTHREAD_MUTEX_INITIALIZER	\
	{ 0, 0, 0, PTHREAD_MUTEX_DEFAULT }

#define	PTHREAD_RECURSIVE_MUTEX_INITIALIZER	\
	{ 0, 0, 0, PTHREAD_MUTEX_RECURSIVE }

#define PTHREAD_ERRORCHECK_MUTEX_INITIALIZER	\
	{ 0, 0, 0, PTHREAD_MUTEX_ERRORCHECK }

#define PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP \
		PTHREAD_RECURSIVE_MUTEX_INITIALIZER

#define PTHREAD_ERRORCHECK_MUTEX_INITIALIZER_NP \
		PTHREAD_ERRORCHECK_MUTEX_INITIALIZER

typedef struct pthread_mutexattr_s
{
	int					_kind;			/*	lock kind*/
	int					_rsv0;			/*	never use*/
} pthread_mutexattr_t;

typedef struct pthread_mutex_s
{
	int					_mlock;
	int					_owner;
	unsigned int		_count;
	int					_flags;
} pthread_mutex_t;

typedef struct pthread_condattr_s
{
	int					_kind;			/*	never use*/
	int					_rsv0;			/*	never use*/
} pthread_condattr_t;

typedef struct pthread_cond_s
{
	int					_cv;			/*	condition variables*/
} pthread_cond_t;

#define	PTHREAD_COND_INITIALIZER {0}

/*	read-write lock types*/
enum
{
	PTHREAD_RWLOCK_PREFER_READER_NP,
	PTHREAD_RWLOCK_PREFER_WRITER_NP,		/*	dummy*/
	PTHREAD_RWLOCK_PREFER_WRITER_NONRECURSIVE_NP,
	PTHREAD_RWLOCK_DEFAULT_NP = PTHREAD_RWLOCK_PREFER_READER_NP
};

#define PTHREAD_RWLOCK_INITIALIZER	\
	{ 0, 0, 0, 0, 0, 0, 0, PTHREAD_RWLOCK_DEFAULT_NP }

typedef struct pthread_rwlockattr_s
{
	int					_kind;			/*	lock kind*/
	int					_rsv0;			/*	never use*/
} pthread_rwlockattr_t;

typedef struct pthread_rwlock_s
{
	int				_lock;
	unsigned int	_nr_readers;		/*	number of holding for rd*/
	unsigned int	_rd_futex;
	unsigned int	_wr_futex;
	unsigned int	_nr_readers_queued;	/*	number of waiting for rd*/
	unsigned int	_nr_writers_queued;	/*	number of waiting for wr*/
	int				_writer;			/*	identity of holding wr*/
	int				_flags;
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

#ifndef HAVE_STRUCT_TIMESPEC
#define HAVE_STRUCT_TIMESPEC 1
/**
 *	POSIX.1b structure for a time value.
 *	This is like a `struct timeval' but
 *	has nanoseconds instead of microseconds.
 */
struct timespec {
	long tv_sec;			/* Seconds.  */
	long tv_nsec;			/* Nanoseconds.  */
};
#endif /* HAVE_STRUCT_TIMESPEC */

#ifdef	__cplusplus
extern	"C" {
#endif

LIBLWPTW_API
int pthread_mutex_init(
	pthread_mutex_t * mutex, const pthread_mutexattr_t *attr);

LIBLWPTW_API
int pthread_mutex_destroy(pthread_mutex_t * mutex);

LIBLWPTW_API
int pthread_mutex_lock(pthread_mutex_t * mutex);

LIBLWPTW_API
int pthread_mutex_trylock(pthread_mutex_t * mutex);

LIBLWPTW_API
int pthread_mutex_timedlock(
	pthread_mutex_t * mutex, const struct timespec * tmout);

LIBLWPTW_API
int pthread_mutex_unlock(pthread_mutex_t * mutex);

LIBLWPTW_API
int pthread_mutexattr_init(pthread_mutexattr_t *attr);

LIBLWPTW_API
int pthread_mutexattr_destroy(pthread_mutexattr_t *attr);

LIBLWPTW_API
int pthread_mutexattr_settype(
	pthread_mutexattr_t *attr, int type);

LIBLWPTW_API
int pthread_mutexattr_gettype(
	pthread_mutexattr_t *attr, int * type);

LIBLWPTW_API
int pthread_cond_init(
	pthread_cond_t * cond, const pthread_condattr_t *attr);

LIBLWPTW_API
int pthread_cond_destroy(pthread_cond_t * cond);

LIBLWPTW_API
int pthread_cond_wait(
	pthread_cond_t * cond, pthread_mutex_t * mutex);

LIBLWPTW_API
int pthread_cond_timedwait(
	pthread_cond_t * cond, pthread_mutex_t * mutex,
	const struct timespec * tmout);

LIBLWPTW_API
int pthread_cond_signal(pthread_cond_t * cond);

LIBLWPTW_API
int pthread_cond_broadcast(pthread_cond_t * cond);

LIBLWPTW_API
int pthread_rwlock_init(
	pthread_rwlock_t * rwlock,
	const pthread_rwlockattr_t * attr);

LIBLWPTW_API
int pthread_rwlock_destroy(pthread_rwlock_t * rwlock);

LIBLWPTW_API
int pthread_rwlock_rdlock(pthread_rwlock_t * rwlock);

LIBLWPTW_API
int pthread_rwlock_tryrdlock(pthread_rwlock_t * rwlock);

LIBLWPTW_API
int pthread_rwlock_timedrdlock(
	pthread_rwlock_t * rwlock, const struct timespec * tmout);

LIBLWPTW_API
int pthread_rwlock_wrlock(pthread_rwlock_t * rwlock);

LIBLWPTW_API
int pthread_rwlock_trywrlock(pthread_rwlock_t * rwlock);

LIBLWPTW_API
int pthread_rwlock_timedwrlock(
	pthread_rwlock_t * rwlock, const struct timespec * tmout);

LIBLWPTW_API
int pthread_rwlock_unlock(pthread_rwlock_t * rwlock);

LIBLWPTW_API
int pthread_rwlockattr_init(pthread_rwlockattr_t * attr);

LIBLWPTW_API
int pthread_rwlockattr_destroy(pthread_rwlockattr_t * attr);

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
