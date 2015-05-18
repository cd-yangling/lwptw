/**
 *	rwlock.h
 *
 *	Copyright (C) 2014 YangLing(yl.tienon@gmail.com)
 *
 *	Description:
 *
 *	Revision History:
 *
 *	2015-05-15 Created By YangLing
 */

#ifndef	__LIBLWPTW_RWLOCK_H__
#define	__LIBLWPTW_RWLOCK_H__

#include "pthread.h"

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

#define	try_hold_wrlock(_rwlock_)				\
	(											\
		(										\
			((_rwlock_)->_writer) |				\
			((_rwlock_)->_nr_readers)			\
		) == 0									\
	)

#endif	/*	__LIBLWPTW_RWLOCK_H__*/
