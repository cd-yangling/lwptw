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

#include "./liblwptw.h"
#include "./rwlock.h"
#include "pthread.h"
#include <errno.h>
#include <windows.h>

LIBLWPTW_API
int pthread_rwlock_rdlock(pthread_rwlock_t * rwlock)
{
	int result = 0;
	int _futex;
	int tid = GetCurrentThreadId();

	lll_lock_acquire(rwlock->_lock);

	for(;;)
	{
		if(can_hold_rdlock(rwlock))
		{
			if(++rwlock->_nr_readers == 0)
			{
				--rwlock->_nr_readers;
				result = EAGAIN;
			}
			break;
		}

		if(rwlock->_writer == tid)
		{
			/*	holding WR by myself. deadlock*/
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
		
		lll_lock_release(rwlock->_lock);

		lll_futex_wait(&rwlock->_rd_futex, _futex);

		lll_lock_acquire(rwlock->_lock);

		--rwlock->_nr_readers_queued;
	}

	lll_lock_release(rwlock->_lock);

	return result;
}
