#include "pthread.h"
#include "tstlwptw.h"
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <windows.h>

/**
 *	EDEADLK test
 */
int rwlock_0501(void)
{
	int result;
	pthread_rwlock_t	rwlock;
	
	result = pthread_rwlock_init(&rwlock, NULL);
	if(result)
		return -1;
	
	result = pthread_rwlock_wrlock(&rwlock);
	if(result)
		return -2;
	
	result = pthread_rwlock_wrlock(&rwlock);
	if(result != EDEADLK)
		return -3;
	
	return 0;
}

int rwlock_0502(void)
{
	int result;
	pthread_rwlock_t	rwlock;
	
	result = pthread_rwlock_init(&rwlock, NULL);
	if(result)
		return -1;
	
	result = pthread_rwlock_timedwrlock(&rwlock, NULL);
	if(result)
		return -2;
	
	result = pthread_rwlock_timedwrlock(&rwlock, NULL);
	if(result != EDEADLK)
		return -3;
	
	return 0;
}

/**
 *	_nr_writers_queued overflow
 */
int rwlock_0503(void)
{
	int result;
	int tid = GetCurrentThreadId();
	pthread_rwlock_t	rwlock;
	
	result = pthread_rwlock_init(&rwlock, NULL);
	if(result)
		return -1;

	rwlock._writer = tid + 1;
	rwlock._nr_writers_queued = UINT_MAX;
	result = pthread_rwlock_wrlock(&rwlock);
	if(result != EAGAIN)
		return -2;
	
	return 0;
}

int rwlock_0504(void)
{
	int result;
	int tid = GetCurrentThreadId();
	pthread_rwlock_t	rwlock;
	
	result = pthread_rwlock_init(&rwlock, NULL);
	if(result)
		return -1;

	rwlock._writer = tid + 1;
	rwlock._nr_writers_queued = UINT_MAX;
	result = pthread_rwlock_timedwrlock(&rwlock, NULL);
	if(result != EAGAIN)
		return -2;
	
	return 0;
}
