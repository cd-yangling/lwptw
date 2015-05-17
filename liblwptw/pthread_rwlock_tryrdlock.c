/**
 *	pthread_rwlock_tryrdlock.c
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
	"@(#) $Id: pthread_rwlock_tryrdlock.c,"
	"v 1.00 2015/05/15 09:00:00 CST yangling Exp $ (LBL)";
#endif

#include "./spinlock.h"
#include "./liblwptw.h"
#include "pthread.h"
#include <errno.h>

/*	��������?*/
#define	is_rwlock_prefer_reader(_rwlock_)		\
	((_rwlock_)->_flags == 0)

/*	û��д�� ���� (û��д�Ŷӻ������)*/
#define	try_hold_rdlock(_rwlock_)				\
	(											\
		((_rwlock_)->_writer == 0) &&			\
		(										\
			(!(_rwlock_)->_nr_writers_queued)||	\
			is_rwlock_prefer_reader((_rwlock_))	\
		)										\
	)

LIBLWPTW_API
int pthread_rwlock_tryrdlock(pthread_rwlock_t * rwlock)
{
	int result = EBUSY;

	spin_acquire((spinlock_t*)&rwlock->_lock);

	if(try_hold_rdlock(rwlock))
	{
		if(++rwlock->_nr_readers == 0)
		{
			--rwlock->_nr_readers;
			result = EAGAIN;
		}
		else
			result = 0;
	}

	spin_release((spinlock_t*)&rwlock->_lock);

	return result;
}
