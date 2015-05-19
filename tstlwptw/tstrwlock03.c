#include "pthread.h"
#include "tstlwptw.h"
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <windows.h>

/**
 *	_nr_reader overflow test
 */
int do_test0301(void)
{
	unsigned int i;
	int result;
	pthread_rwlock_t	rwlock;

	_total_cnt = UINT_MAX;

	result = pthread_rwlock_init(&rwlock, NULL);
	if(result)
		return -1;

	for(i = 0; i < UINT_MAX;i++)
	{
		result = pthread_rwlock_rdlock(&rwlock);
		if(result)
			return -2;

		_cur_finish = i;

	}

	result = pthread_rwlock_rdlock(&rwlock);
	if(result != EAGAIN)
		return -3;

	return 0;
}

int do_test0302(void)
{
	unsigned int i;
	int result;
	pthread_rwlock_t	rwlock;
	
	_total_cnt = UINT_MAX;
	
	result = pthread_rwlock_init(&rwlock, NULL);
	if(result)
		return -1;
	
	for(i = 0; i < UINT_MAX;i++)
	{
		result = pthread_rwlock_tryrdlock(&rwlock);
		if(result)
			return -2;
		
		_cur_finish = i;
		
	}
	
	result = pthread_rwlock_tryrdlock(&rwlock);
	if(result != EAGAIN)
		return -3;
	
	return 0;
}

int do_test0303(void)
{
	unsigned int i;
	int result;
	pthread_rwlock_t	rwlock;
	
	_total_cnt = UINT_MAX;
	
	result = pthread_rwlock_init(&rwlock, NULL);
	if(result)
		return -1;
	
	for(i = 0; i < UINT_MAX;i++)
	{
		result = pthread_rwlock_timedrdlock(&rwlock, NULL);
		if(result)
			return -2;
		
		_cur_finish = i;
		
	}
	
	result = pthread_rwlock_timedrdlock(&rwlock, NULL);
	if(result != EAGAIN)
		return -3;
	
	return 0;
}

/**
 *	EDEADLK test
 */
int do_test0304(void)
{
	int result;
	pthread_rwlock_t	rwlock;

	result = pthread_rwlock_init(&rwlock, NULL);
	if(result)
		return -1;

	result = pthread_rwlock_wrlock(&rwlock);
	if(result)
		return -2;

	result = pthread_rwlock_rdlock(&rwlock);
	if(result != EDEADLK)
		return -3;

	return 0;
}

int do_test0305(void)
{
	int result;
	pthread_rwlock_t	rwlock;
	
	result = pthread_rwlock_init(&rwlock, NULL);
	if(result)
		return -1;
	
	result = pthread_rwlock_wrlock(&rwlock);
	if(result)
		return -2;
	
	result = pthread_rwlock_timedrdlock(&rwlock, NULL);
	if(result != EDEADLK)
		return -3;
	
	return 0;
}

/**
 *	_nr_readers_queued overflow test
 */
int do_test0306(void)
{
	int result;
	int tid = GetCurrentThreadId();
	pthread_rwlock_t	rwlock = PTHREAD_RWLOCK_INITIALIZER;

	rwlock._writer = tid + 1;
	rwlock._nr_readers_queued = UINT_MAX;
	result = pthread_rwlock_rdlock(&rwlock);
	if(result != EAGAIN)
		return -1;
	else
		return 0;
}

int do_test0307(void)
{
	int result;
	int tid = GetCurrentThreadId();
	pthread_rwlock_t	rwlock = PTHREAD_RWLOCK_INITIALIZER;

	rwlock._writer = tid + 1;
	rwlock._nr_readers_queued = UINT_MAX;
	result = pthread_rwlock_timedrdlock(&rwlock, NULL);
	if(result != EAGAIN)
		return -1;
	else
		return 0;
}
