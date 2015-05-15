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
 *	implementation href:
 *	http://people.redhat.com/drepper/futex.pdf
 */

#ifndef lint
static const char rcsid[] =
	"@(#) $Id: pthread_mutex_unlock.c,"
	"v 1.00 2015/05/15 09:00:00 CST yangling Exp $ (LBL)";
#endif

#include "./atomic.h"
#include "./liblwptw.h"
#include "futex/futex.h"
#include "pthread.h"

/**
 *	0:	unlocked
 *	1:	locked, no waiters
 *	2:	locked, one or more waiters
 */
LIBLWPTW_API
int pthread_mutex_unlock(pthread_mutex_t * mutex)
{
	if(atomic_dec_and_return(&mutex->_futex) != 1)
	{
		*(volatile int *)(&mutex->_futex) = 0;
		lll_futex_wake(&mutex->_futex, 1);
	}

	return 0;
}
