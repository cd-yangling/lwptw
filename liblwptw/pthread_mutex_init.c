/**
 *	pthread_mutex_init.c
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
	"@(#) $Id: pthread_mutex_init.c,"
	"v 1.00 2015/05/15 09:00:00 CST yangling Exp $ (LBL)";
#endif

#include "pthread.h"
#include <string.h>

LIBLWPTW_API
int pthread_mutex_init(
	pthread_mutex_t * mutex, const pthread_mutexattr_t *attr)
{
	int type;

	memset(mutex, 0, sizeof(pthread_mutex_t));

	type = (attr) ? attr->_kind : PTHREAD_MUTEX_DEFAULT;

	switch(type)
	{
	case PTHREAD_MUTEX_NORMAL:
	case PTHREAD_MUTEX_RECURSIVE:
	case PTHREAD_MUTEX_ERRORCHECK:
		mutex->_flags = type;
		break;
	default:
		mutex->_flags = PTHREAD_MUTEX_DEFAULT;
		break;
	}

	return 0;
}
