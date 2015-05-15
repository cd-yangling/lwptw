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
 *	implementation href:
 *	http://people.redhat.com/drepper/futex.pdf
 */

#ifndef lint
static const char rcsid[] =
	"@(#) $Id: pthread_mutex_lock.c,"
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
int pthread_mutex_lock(pthread_mutex_t * mutex)
{
	int		c;
	
	if((c = atomic_cmpxchg(
				&mutex->_futex, 0, 1)) != 0)
	{
		if(c != 2)
			c = atomic_xchg(&mutex->_futex, 2);
		
		while(c != 0)
		{
			lll_futex_wait(&mutex->_futex, 2);
			c = atomic_xchg(&mutex->_futex, 2);
		}
	}

	return 0;
}
