#include "pthread.h"
#include "tstlwptw.h"
#include <errno.h>
#include <limits.h>
#include <stdio.h>

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
