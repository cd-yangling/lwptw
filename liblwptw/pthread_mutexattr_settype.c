/**
 *	pthread_mutexattr_settype.c
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
	"@(#) $Id: pthread_mutexattr_settype.c,"
	"v 1.00 2015/05/15 09:00:00 CST yangling Exp $ (LBL)";
#endif

#include "pthread.h"
#include <errno.h>

LIBLWPTW_API
int pthread_mutexattr_settype(
	pthread_mutexattr_t *attr, int type)
{
	switch(type)
	{
	case PTHREAD_MUTEX_NORMAL:
	case PTHREAD_MUTEX_RECURSIVE:
	case PTHREAD_MUTEX_ERRORCHECK:
		attr->_kind = type;
		return 0;
	default:
		return EINVAL;
	}
}
