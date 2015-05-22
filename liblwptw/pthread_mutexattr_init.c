/**
 *	pthread_mutexattr_init.c
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
	"@(#) $Id: pthread_mutexattr_init.c,"
	"v 1.00 2015/05/15 09:00:00 CST yangling Exp $ (LBL)";
#endif

#include "pthread.h"

LIBLWPTW_API
int pthread_mutexattr_init(pthread_mutexattr_t *attr)
{
	attr->_kind = PTHREAD_MUTEX_NORMAL;
	attr->_rsv0 = 0;

	return 0;
}
