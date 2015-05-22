/**
 *	pthread_mutex_lock.c
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
	"@(#) $Id: pthread_mutex_lock.c,"
	"v 1.00 2015/05/15 09:00:00 CST yangling Exp $ (LBL)";
#endif

#include "./liblwptw.h"
#include "pthread.h"
#include <errno.h>
#include <windows.h>

LIBLWPTW_API
int pthread_mutex_lock(pthread_mutex_t * mutex)
{
	switch(mutex->_flags)
	{
	case PTHREAD_MUTEX_NORMAL:
		{
			lll_lock_acquire(mutex->_futex);
		}
		return 0;
	case PTHREAD_MUTEX_RECURSIVE:
		{
			/*	recursive mutex*/
			int tid = GetCurrentThreadId();

			if(mutex->_owner == tid)
			{
				if(0 == (mutex->_count + 1))
					return EAGAIN;

				++mutex->_count;

				return 0;
			}

			lll_lock_acquire(mutex->_futex);

			mutex->_owner = tid;
			mutex->_count = 1;
		}
		return 0;
	case PTHREAD_MUTEX_ERRORCHECK:
		{
			int tid = GetCurrentThreadId();

			if(mutex->_owner == tid)
				return EDEADLK;

			lll_lock_acquire(mutex->_futex);

			mutex->_owner = tid;
			mutex->_count = 1;
		}
		return 0;
	default:
		return EINVAL;
	}
}
