/**
 *	pthread_rwlock_unlock.c
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
	"@(#) $Id: pthread_rwlock_unlock.c,"
	"v 1.00 2015/05/15 09:00:00 CST yangling Exp $ (LBL)";
#endif

#include "./spinlock.h"
#include "./liblwptw.h"
#include "futex/futex.h"
#include "pthread.h"
#include <errno.h>
#include <limits.h>	/*	INT_MAX required*/

LIBLWPTW_API
int pthread_rwlock_unlock(pthread_rwlock_t * rwlock)
{
	spin_acquire((spinlock_t*)&rwlock->_lock);

	if(rwlock->_writer)
		rwlock->_writer = 0;
	else
		--rwlock->_nr_readers;

	if(rwlock->_nr_readers == 0)
	{
		if(rwlock->_nr_writers_queued)
		{
			++rwlock->_wr_futex;
			spin_release((spinlock_t*)&rwlock->_lock);
			lll_futex_wake(&rwlock->_wr_futex, 1);
			return 0;
		}
		else if(rwlock->_nr_readers_queued)
		{
			++rwlock->_rd_futex;
			spin_release((spinlock_t*)&rwlock->_lock);
			lll_futex_wake(&rwlock->_rd_futex, INT_MAX);
			return 0;
		}
	}

	spin_release((spinlock_t*)&rwlock->_lock);

	return 0;
}
