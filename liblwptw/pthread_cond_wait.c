/**
 *	pthread_cond_wait.c
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
	"@(#) $Id: pthread_cond_wait.c,"
	"v 1.00 2015/05/15 09:00:00 CST yangling Exp $ (LBL)";
#endif

#include "pthread.h"
#include "liblwptw.h"

LIBLWPTW_API
int pthread_cond_wait(
	pthread_cond_t * cond, pthread_mutex_t * mutex)
{
	int _cv;
	int result;

	_cv = *(volatile int *)(&cond->_cv);

	result = pthread_mutex_unlock(mutex);
	if(result)
		return result;

	lll_futex_wait(&(cond->_cv), _cv);

	return pthread_mutex_lock(mutex);
}
