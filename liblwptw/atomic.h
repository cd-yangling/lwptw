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
 *	ԭ�ӵĶ� @minuend ָ��� 32bit
 *	��ֵ��1 Ȼ�󷵻� @minuend ָ��
 *	��1֮ǰ��ֵ
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
 *	ԭ�ӵ�ʹ�� @new_val ���� @old_val 
 *	ָ���32bit��ֵ Ȼ�󷵻� @old_val
 *	����ǰ��ֵ
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
 *	ԭ�ӵ�ʹ�� @cml_val �� @old_val
 *	ָ���32bit��ֵ, ��Ƚ���.���Ȳ�����
 *	Ȼ�󷵻� @old_val ����֮ǰ��ֵ
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
