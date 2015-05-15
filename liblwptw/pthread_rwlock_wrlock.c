/**
 *	pthread_rwlock_wrlock.c
 *
 *	Copyright (C) 2014 YangLing(yl.tienon@gmail.com)
 *
 *	Description:
 *
 *	Revision History:
 *
 *	2015-05-15 Created By YangLing
 *
 *	implementation ²Î¿¼ glibc
 */

#ifndef lint
static const char rcsid[] =
	"@(#) $Id: pthread_rwlock_wrlock.c,"
	"v 1.00 2015/05/15 09:00:00 CST yangling Exp $ (LBL)";
#endif

#include "./spinlock.h"
#include "./liblwptw.h"
#include "pthread.h"
#include <errno.h>
#include <windows.h>

#define	try_hold_wrlock(_rwlock_)		\
	(									\
		(								\
			((_rwlock_)->_writer) |		\
			((_rwlock_)->_nr_readers)	\
		) == 0							\
	)

static int
pthread_rwlock_wrlock_slow(pthread_rwlock_t * rwlock, int tid)
{
	int result = 0;
	int _futex;

	for(;;)
	{
		if(rwlock->_writer == tid)
		{
			/*	already held wrlock. deadlock*/
			result = EDEADLK;
			break;
		}

		if(++rwlock->_nr_writers_queued == 0)
		{
			--rwlock->_nr_writers_queued;
			result = EAGAIN;
			break;
		}

		_futex = rwlock->_wr_futex;

		spin_release((spinlock_t*)&rwlock->_lock);

		lll_futex_wait(&rwlock->_wr_futex, _futex);

		spin_acquire((spinlock_t*)&rwlock->_lock);

		--rwlock->_nr_writers_queued;

		if(try_hold_wrlock(rwlock))
		{
			rwlock->_writer = tid;
			break;
		}
	}

	spin_release((spinlock_t*)&rwlock->_lock);

	return result;
}

LIBLWPTW_API
int pthread_rwlock_wrlock(pthread_rwlock_t * rwlock)
{
	int tid = GetCurrentThreadId();

	spin_acquire((spinlock_t*)&rwlock->_lock);

	if(try_hold_wrlock(rwlock))
	{
		rwlock->_writer = tid;

		spin_release((spinlock_t*)&rwlock->_lock);
		return 0;
	}

	return pthread_rwlock_wrlock_slow(rwlock, tid);
}
