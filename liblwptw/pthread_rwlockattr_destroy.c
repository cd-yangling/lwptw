/**
 *	pthread_rwlockattr_destroy.c
 *
 *	Copyright (C) 2014 YangLing(yl.tienon@gmail.com)
 *
 *	Description:
 *
 *	Revision History:
 *
 *	2015-05-15 Created By YangLing
 *
 *	implementation �ο� glibc
 */

#ifndef lint
static const char rcsid[] =
	"@(#) $Id: pthread_rwlockattr_destroy.c,"
	"v 1.00 2015/05/15 09:00:00 CST yangling Exp $ (LBL)";
#endif

#include "pthread.h"

LIBLWPTW_API int
pthread_rwlockattr_destroy(pthread_rwlockattr_t * attr)
{
  return 0;	/*	just let our compiler happy*/
}
