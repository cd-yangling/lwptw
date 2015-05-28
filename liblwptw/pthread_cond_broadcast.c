/**
 *	pthread_cond_broadcast.c
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
	"@(#) $Id: pthread_cond_broadcast.c,"
	"v 1.00 2015/05/15 09:00:00 CST yangling Exp $ (LBL)";
#endif

#include "pthread.h"
#include "./liblwptw.h"
#include "./atomic.h"
#include <limits.h>

LIBLWPTW_API
int pthread_cond_broadcast(pthread_cond_t * cond)
{
	atomic_inc(&(cond->_cv));

	lll_futex_wake(&(cond->_cv), INT_MAX);

	return 0;
}
