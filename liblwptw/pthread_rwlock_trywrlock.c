/**
 *	pthread_rwlock_trywrlock.c
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
	"@(#) $Id: pthread_rwlock_trywrlock.c,"
	"v 1.00 2015/05/15 09:00:00 CST yangling Exp $ (LBL)";
#endif

#include "./liblwptw.h"
#include "./rwlock.h"
#include "pthread.h"
#include <errno.h>
#include <windows.h>

LIBLWPTW_API
int pthread_rwlock_trywrlock(pthread_rwlock_t * rwlock)
{
	int result = EBUSY;
	int tid = GetCurrentThreadId();

	lll_lock_acquire(rwlock->_lock);

	if(can_hold_wrlock(rwlock))
	{
		rwlock->_writer = tid;
		result = 0;
	}

	lll_lock_release(rwlock->_lock);

	return result;
}
