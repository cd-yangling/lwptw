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
#include "./rwlock.h"
#include "pthread.h"
#include <errno.h>
#include <windows.h>

LIBLWPTW_API
int pthread_rwlock_wrlock(pthread_rwlock_t * rwlock)
{
	int result = 0;
	int _futex;
	int tid = GetCurrentThreadId();

	spin_acquire((spinlock_t*)&rwlock->_lock);

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

		spin_release((spinlock_t*)&rwlock->_lock);

		lll_futex_wait(&rwlock->_wr_futex, _futex);

		spin_acquire((spinlock_t*)&rwlock->_lock);

		--rwlock->_nr_writers_queued;
	}

	return result;
}
