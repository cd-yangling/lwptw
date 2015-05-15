/**
 *	atomic.h
 *
 *	Copyright (C) 2014 YangLing(yl.tienon@gmail.com)
 *
 *	Description:
 *
 *	Revision History:
 *
 *	2015-05-15 Created By YangLing
 */

#ifndef	__LIBLWPTW_ATOMIC_H__
#define	__LIBLWPTW_ATOMIC_H__

#ifndef	always_inline
#define	always_inline	__forceinline
#endif

/**
 *	原子的对 @minuend 指向的 32bit
 *	的值减1 然后返回 @minuend 指向
 *	减1之前的值
 */
always_inline static
int atomic_dec_and_return(int * minuend)
{
	__asm
	{
		mov			edx, dword ptr [minuend];
		mov			eax, 0xFFFFFFFF;
		lock xadd dword ptr [edx], eax;
	}
}

/**
 *	原子的使用 @new_val 交换 @old_val 
 *	指向的32bit的值 然后返回 @old_val
 *	交换前的值
 */
always_inline static
int atomic_xchg(int * old_val, int new_val)
{
	_asm
	{
		mov			ecx, dword ptr [old_val];
		mov			eax, new_val;
		lock xchg	dword ptr [ecx], eax;
	}
}

/**
 *	原子的使用 @cml_val 与 @old_val
 *	指向的32bit的值, 相等交换.不等不交换
 *	然后返回 @old_val 操作之前的值
 */
always_inline static
int atomic_cmpxchg(
	int * old_val, int cmp_val, int new_val)
{
	_asm
	{
		mov				ecx, dword ptr [old_val];
		mov				edx, new_val;
		mov				eax, cmp_val;
		lock cmpxchg	dword ptr [ecx],edx;
	}
}

#endif	/*	__LIBLWPTW_LWPTW_H__*/
