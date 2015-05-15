/**
 *	pthread_rwlock_rdlock.c
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
	"@(#) $Id: pthread_rwlock_rdlock.c,"
	"v 1.00 2015/05/15 09:00:00 CST yangling Exp $ (LBL)";
#endif

#include "./spinlock.h"
#include "./liblwptw.h"
#include "pthread.h"
#include <errno.h>
#include <windows.h>

#define	try_hold_rdlock(_rwlock_)				\
	(											\
		(										\
			((_rwlock_)->_writer) |				\
			((_rwlock_)->_nr_writers_queued)	\
		) == 0									\
	)

static int
pthread_rwlock_rdlock_slow(pthread_rwlock_t * rwlock)
{
	int result = 0;
	int _futex;
	int tid = GetCurrentThreadId();

	for(;;)
	{
		if(rwlock->_writer == tid)
		{
			/*	already held wrlock. deadlock*/
			result = EDEADLK;
			break;
		}

		if(++rwlock->_nr_readers_queued == 0)
		{
			--rwlock->_nr_readers_queued;
			result = EAGAIN;
			break;
		}

		_futex = rwlock->_rd_futex;

		spin_release((spinlock_t*)&rwlock->_lock);

		lll_futex_wait(&rwlock->_rd_futex, _futex);

		spin_acquire((spinlock_t*)&rwlock->_lock);

		--rwlock->_nr_readers_queued;

		if(try_hold_rdlock(rwlock))
		{
			if(++rwlock->_nr_readers == 0)
			{
				--rwlock->_nr_readers;
				result = EAGAIN;
			}
			break;
		}
	}

	spin_release((spinlock_t*)&rwlock->_lock);

	return result;
}

LIBLWPTW_API
int pthread_rwlock_rdlock(pthread_rwlock_t * rwlock)
{
	int result = 0;

	spin_acquire((spinlock_t*)&rwlock->_lock);

	if(try_hold_rdlock(rwlock))
	{
		if(++rwlock->_nr_readers == 0)
		{
			--rwlock->_nr_readers;
			result = EAGAIN;
		}

		spin_release((spinlock_t*)&rwlock->_lock);
		return result;
	}

	return pthread_rwlock_rdlock_slow(rwlock);
}
