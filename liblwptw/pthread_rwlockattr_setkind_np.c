/**
 *	pthread_rwlockattr_setkind_np.c
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
	"@(#) $Id: pthread_rwlockattr_setkind_np.c,"
	"v 1.00 2015/05/15 09:00:00 CST yangling Exp $ (LBL)";
#endif

#include "pthread.h"
#include <errno.h>

LIBLWPTW_API
int pthread_rwlockattr_setkind_np(
	pthread_rwlockattr_t * attr, int pref)
{
	switch(pref)
	{
	case PTHREAD_RWLOCK_PREFER_READER_NP:
	case PTHREAD_RWLOCK_PREFER_WRITER_NP:
	case PTHREAD_RWLOCK_PREFER_WRITER_NONRECURSIVE_NP:
		attr->_kind = pref;
		return 0;
	default:
		return EINVAL;
	}
}
