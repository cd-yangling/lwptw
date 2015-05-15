/**
 *	pthread_rwlockattr_getkind_np.c
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
	"@(#) $Id: pthread_rwlockattr_getkind_np.c,"
	"v 1.00 2015/05/15 09:00:00 CST yangling Exp $ (LBL)";
#endif

#include "pthread.h"

LIBLWPTW_API
int pthread_rwlockattr_getkind_np(
	const pthread_rwlockattr_t * attr, int * pref)
{
	*pref = attr->_kind;

	return 0;
}
