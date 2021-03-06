/**
 *	liblwptw.h
 *
 *	Copyright (C) 2014 YangLing(yl.tienon@gmail.com)
 *
 *	Description:
 *
 *	Revision History:
 *
 *	2015-05-15 Created By YangLing
 */

#ifndef	__LIBLWPTW_LWPTW_H__
#define	__LIBLWPTW_LWPTW_H__

#include <stdio.h>
#include "futex/futex.h"
#include "./atomic.h"

static void lll_futex_wait(int * addr, int val)
{
	(void) futex(addr, FUTEX_WAIT, val, NULL);
}

static void lll_futex_wake(int * addr, int nr_wake)
{
	(void) futex(addr, FUTEX_WAKE, nr_wake, NULL);
}

static int lll_futex_timed_wait(
	int * addr, int val, const struct timespec *tmout)
{
	return futex(addr, FUTEX_WAIT, val, tmout);
}

/**
 *	0:	unlocked
 *	1:	locked, no waiters
 *	2:	locked, one or more waiters
 *
 *	more implementation details see below link
 *	http://people.redhat.com/drepper/futex.pdf
 */
always_inline static
int __lll_lock_try_acquire(int * lock)
{
	return atomic_cmpxchg(lock, 0, 1);
}

#define	lll_lock_try_acquire(_lock_)					\
	__lll_lock_try_acquire(&(_lock_))

always_inline static
int __lll_lock_timed_acquire(
	int * lock, const struct timespec *abstime)
{
	int c;
	int err;

	if((c = atomic_cmpxchg(lock, 0, 1)) != 0) {
		if(c != 2)
			c = atomic_xchg(lock, 2);
		while(c != 0) {
			err = lll_futex_timed_wait(
						lock, 2, abstime);
			if(-E_FUTEX_TIMEOUT == err)
				return 1;
			c = atomic_xchg(lock, 2);
		}
	}

	return 0;
}

#define lll_lock_timed_acquire(_lock_, _abstime_)		\
	__lll_lock_timed_acquire(&(_lock_), (_abstime_))

#define	lll_lock_acquire(_lock_)						\
	do {												\
		int c;											\
		int *__futex = &(_lock_);						\
		if((c = atomic_cmpxchg(__futex, 0, 1)) != 0) {	\
			if(c != 2)									\
				c = atomic_xchg(__futex, 2);			\
			while(c != 0) {								\
				lll_futex_wait(__futex, 2);				\
				c = atomic_xchg(__futex, 2);			\
			}											\
		}												\
	} while(0)

#define	lll_lock_release(_lock_)						\
	do {												\
		int *__futex = &(_lock_);						\
		if(atomic_dec_and_return(__futex) != 1)	 {		\
			*(volatile int *)(__futex) = 0;				\
			lll_futex_wake(__futex, 1);					\
		}												\
	} while(0)

#endif	/*	__LIBLWPTW_LWPTW_H__*/
