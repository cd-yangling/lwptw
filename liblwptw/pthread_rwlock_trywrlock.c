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

#include "./spinlock.h"
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

LIBLWPTW_API
int pthread_rwlock_trywrlock(pthread_rwlock_t * rwlock)
{
	int result = EBUSY;
	int tid = GetCurrentThreadId();

	spin_acquire((spinlock_t*)&rwlock->_lock);

	if(try_hold_wrlock(rwlock))
	{
		rwlock->_writer = tid;
		result = 0;
	}

	spin_release((spinlock_t*)&rwlock->_lock);

	return result;
}
