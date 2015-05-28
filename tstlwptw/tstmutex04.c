#include "pthread.h"
#include "tstlwptw.h"
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <windows.h>
#include <process.h>

static volatile int		_0405_over = 0;

static void _0401_thread(void * args)
{
	int result;
	pthread_mutex_t * mutex = 
		(pthread_mutex_t*)args;

	result = pthread_mutex_lock(mutex);
	if(result)
		return;

	result = pthread_mutex_unlock(mutex);
	if(result)
		return;

	_0405_over = 1;

	return;
}

int mutex_0401(void)
{
	pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
	int result;

	result = pthread_mutex_lock(&mutex);
	if(result)
		return -1;

	if(mutex._mlock != 1)
		return -2;

	_0405_over = 0;
	_beginthread(_0401_thread, 0, &mutex);

	while(mutex._mlock != 2)
		Sleep(1);

	result = pthread_mutex_unlock(&mutex);
	if(result)
		return -3;

	while(mutex._mlock != 0)
		Sleep(1);

	while(_0405_over == 0)
		Sleep(1);

	return 0;
}

int mutex_0402(void)
{
	pthread_mutex_t mutex = PTHREAD_RECURSIVE_MUTEX_INITIALIZER;
	int result;
	unsigned int i, j;
	int tid = GetCurrentThreadId();

	_total_cnt = UINT_MAX * (__int64)2;

	for(i = 0; i < UINT_MAX ;i++)
	{
		result = pthread_mutex_lock(&mutex);
		if(result)
			return -1;

		if(tid != mutex._owner)
			return -2;

		if(mutex._count - 1 != i)
			return -3;

		if(mutex._mlock != 1)
			return -4;

		if(PTHREAD_MUTEX_RECURSIVE != mutex._flags)
			return -5;

		_cur_finish++;
	}

	result = pthread_mutex_lock(&mutex);
	if(result != EAGAIN)
		return -6;

	if(tid != mutex._owner)
		return -7;

	if(mutex._count != 0xFFFFFFFF)
		return -8;

	if(mutex._mlock != 1)
		return -9;

	if(PTHREAD_MUTEX_RECURSIVE != mutex._flags)
		return -10;

	for(i = 0, j = UINT_MAX - 1; i < UINT_MAX - 1;i++, j--)
	{
		result = pthread_mutex_unlock(&mutex);
		if(result)
			return -11;

		if(tid != mutex._owner)
			return -12;
		
		if(mutex._count != j)
			return -13;
		
		if(mutex._mlock != 1)
			return -14;

		if(PTHREAD_MUTEX_RECURSIVE != mutex._flags)
			return -15;

		_cur_finish++;
	}

	result = pthread_mutex_unlock(&mutex);
	if(result)
		return -16;

	if(0 != mutex._owner)
		return -17;

	if(0 != mutex._count)
		return -18;

	if(0 != mutex._mlock)
		return -19;

	if(PTHREAD_MUTEX_RECURSIVE != mutex._flags)
		return -20;

	result = pthread_mutex_unlock(&mutex);
	if(result != EPERM)
		return -21;

	if(0 != mutex._owner)
		return -22;
	
	if(0 != mutex._count)
		return -23;
	
	if(0 != mutex._mlock)
		return -24;
	
	if(PTHREAD_MUTEX_RECURSIVE != mutex._flags)
		return -25;

	_cur_finish = UINT_MAX * (__int64)2;

	return 0;
}

int mutex_0403(void)
{
	pthread_mutex_t mutex = PTHREAD_ERRORCHECK_MUTEX_INITIALIZER;
	int result;
	int tid = GetCurrentThreadId();

	result = pthread_mutex_lock(&mutex);
	if(result)
		return -1;

	if(tid != mutex._owner)
		return -2;

	if(1 != mutex._mlock)
		return -3;

	if(PTHREAD_MUTEX_ERRORCHECK != mutex._flags)
		return -4;

	result = pthread_mutex_lock(&mutex);
	if(result != EDEADLK)
		return -5;

	if(tid != mutex._owner)
		return -6;
	
	if(1 != mutex._mlock)
		return -7;
	
	if(PTHREAD_MUTEX_ERRORCHECK != mutex._flags)
		return -8;

	result = pthread_mutex_unlock(&mutex);
	if(result)
		return -9;

	if(0 != mutex._owner)
		return -10;

	if(0 != mutex._mlock)
		return -11;

	if(PTHREAD_MUTEX_ERRORCHECK != mutex._flags)
		return -12;

	result = pthread_mutex_unlock(&mutex);
	if(result != EPERM)
		return -13;

	if(0 != mutex._owner)
		return -14;
	
	if(0 != mutex._mlock)
		return -15;
	
	if(PTHREAD_MUTEX_ERRORCHECK != mutex._flags)
		return -16;

	return 0;
}
