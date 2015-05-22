/**
 *	pthread_rwlock_tryrdlock.c
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
	"@(#) $Id: pthread_rwlock_tryrdlock.c,"
	"v 1.00 2015/05/15 09:00:00 CST yangling Exp $ (LBL)";
#endif

#include "./liblwptw.h"
#include "./rwlock.h"
#include "pthread.h"
#include <errno.h>

LIBLWPTW_API
int pthread_rwlock_tryrdlock(pthread_rwlock_t * rwlock)
{
	int result = EBUSY;

	lll_lock_acquire(rwlock->_lock);

	if(can_hold_rdlock(rwlock))
	{
		if(++rwlock->_nr_readers == 0)
		{
			--rwlock->_nr_readers;
			result = EAGAIN;
		}
		else
			result = 0;
	}

	lll_lock_release(rwlock->_lock);

	return result;
}
