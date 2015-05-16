/**
 *	pthread_rwlock_init.c
 *
 *	Copyright (C) 2014 YangLing(yl.tienon@gmail.com)
 *
 *	Description:
 *
 *	Revision History:
 *
 *	2015-05-15 Created By YangLing
 *
 *	implementation 参考 glibc
 */

#ifndef lint
static const char rcsid[] =
	"@(#) $Id: pthread_rwlock_init.c,"
	"v 1.00 2015/05/15 09:00:00 CST yangling Exp $ (LBL)";
#endif

#include "pthread.h"
#include <string.h>

LIBLWPTW_API
int pthread_rwlock_init(
	pthread_rwlock_t * rwlock,
	const pthread_rwlockattr_t * attr)
{
	int	kind;

	memset(rwlock, 0, sizeof(pthread_rwlock_t));

	/**
	 *	FUCK!!! 我现在终于弄明白了为什么当年
	 *	pthread_rwlockattr_setkind_np 设置为
	 *	PTHREAD_RWLOCK_PREFER_WRITER_NP 没用
	 *	因为在glibc中就完全忽略掉了这个参数.
	 *	Okay!!! 我还是保持和glibc一样的行为.
	 */
	kind = (attr) ? attr->_kind : PTHREAD_RWLOCK_DEFAULT_NP;
	rwlock->_flags =
		(kind == PTHREAD_RWLOCK_PREFER_WRITER_NONRECURSIVE_NP);

	/**
	 *	shared filed not support!!
	 */
	return 0;
}
