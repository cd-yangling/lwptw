/**
 *	pthread_mutex_unlock.c
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
	"@(#) $Id: pthread_mutex_unlock.c,"
	"v 1.00 2015/05/15 09:00:00 CST yangling Exp $ (LBL)";
#endif

#include "./liblwptw.h"
#include "pthread.h"
#include <errno.h>
#include <windows.h>

LIBLWPTW_API
int pthread_mutex_unlock(pthread_mutex_t * mutex)
{
	switch(mutex->_flags)
	{
	case PTHREAD_MUTEX_NORMAL:
		{
normal:
			lll_lock_release(mutex->_mlock);
			return 0;
		}
	case PTHREAD_MUTEX_RECURSIVE:
		{
			int tid = GetCurrentThreadId();

			if(mutex->_owner != tid)
				return EPERM;

			if(--mutex->_count != 0)
				return 0;	/*	still hold the mutex*/

			goto normal;
		}
		return 0;
	case PTHREAD_MUTEX_ERRORCHECK:
		{
			int tid = GetCurrentThreadId();

			if((mutex->_owner != tid) ||
				(!(mutex->_mlock != 0)))
				return EPERM;

			goto normal;
		}
		break;
	default:
		return EINVAL;
	}
}
