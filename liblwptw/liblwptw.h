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

#endif	/*	__LIBLWPTW_LWPTW_H__*/
