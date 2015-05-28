/**
 *	pthread_cond_timedwait.c
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
	"@(#) $Id: pthread_cond_timedwait.c,"
	"v 1.00 2015/05/15 09:00:00 CST yangling Exp $ (LBL)";
#endif

#include "pthread.h"
#include "liblwptw.h"

LIBLWPTW_API
int pthread_cond_timedwait(
	pthread_cond_t * cond, pthread_mutex_t * mutex,
	const struct timespec * tmout)
{
	int _cv;
	int result;

	_cv = *(volatile int *)(&cond->_cv);

	result = pthread_mutex_unlock(mutex);
	if(result)
		return result;

	result = lll_futex_timed_wait(&(cond->_cv), _cv, tmout);
	if(-E_FUTEX_TIMEOUT == result)
		result = ETIMEDOUT;
	else
		result = 0;

	return result;
}
