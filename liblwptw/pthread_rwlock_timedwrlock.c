/**
 *	pthread_rwlock_timedwrlock.c
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
	"@(#) $Id: pthread_rwlock_timedwrlock.c,"
	"v 1.00 2015/05/15 09:00:00 CST yangling Exp $ (LBL)";
#endif

#include "./liblwptw.h"
#include "./rwlock.h"
#include "pthread.h"
#include <errno.h>
#include <windows.h>

LIBLWPTW_API
int pthread_rwlock_timedwrlock(
	pthread_rwlock_t * rwlock, const struct timespec * tmout)
{
	int result = 0;
	int _futex;
	int tid = GetCurrentThreadId();
	int err;

	lll_lock_acquire(rwlock->_lock);

	for(;;)
	{
		if(can_hold_wrlock(rwlock))
		{
			rwlock->_writer = tid;
			break;
		}

		if(rwlock->_writer == tid)
		{
			/*	holding WR by myself. deadlock*/
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
		
		lll_lock_release(rwlock->_lock);
		
		err = lll_futex_timed_wait(
				&rwlock->_wr_futex, _futex, tmout);
		
		lll_lock_acquire(rwlock->_lock);
		
		--rwlock->_nr_writers_queued;

		if(err == -E_FUTEX_TIMEOUT)
		{
			result = ETIMEDOUT;
			break;
		}
	}

	lll_lock_release(rwlock->_lock);

	return result;
}
