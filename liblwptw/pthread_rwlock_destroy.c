/**
 *	pthread_rwlock_destroy.c
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
	"@(#) $Id: pthread_rwlock_destroy.c,"
	"v 1.00 2015/05/15 09:00:00 CST yangling Exp $ (LBL)";
#endif

#include "pthread.h"

LIBLWPTW_API
int pthread_rwlock_destroy(pthread_rwlock_t * rwlock)
{
  return 0;	/*	just let our compiler happy*/
}
