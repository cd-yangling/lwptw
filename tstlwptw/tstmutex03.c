#include "pthread.h"
#include "tstlwptw.h"
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <windows.h>

/**
 *	PTHREAD_MUTEX_INITIALIZER
 */
int mutex_0301(void)
{
	pthread_mutex_t	mutex = PTHREAD_MUTEX_INITIALIZER;
	int result;

	result = pthread_mutex_lock(&mutex);
	if(result)
		return -1;

	if(1 != mutex._mlock)
		return -4;

	return pthread_mutex_unlock(&mutex);
}

int mutex_0302(void)
{
	pthread_mutex_t	mutex = PTHREAD_MUTEX_INITIALIZER;
	int result;
	
	result = pthread_mutex_trylock(&mutex);
	if(result)
		return -1;

	if(1 != mutex._mlock)
		return -4;
	
	return pthread_mutex_unlock(&mutex);
}

int mutex_0303(void)
{
	pthread_mutex_t	mutex = PTHREAD_MUTEX_INITIALIZER;
	int result;
	
	result = pthread_mutex_timedlock(&mutex, NULL);
	if(result)
		return -1;

	if(1 != mutex._mlock)
		return -4;
	
	return pthread_mutex_unlock(&mutex);
}

/**
 *	PTHREAD_RECURSIVE_MUTEX_INITIALIZER
 */
int mutex_0304(void)
{
	pthread_mutex_t	mutex = PTHREAD_RECURSIVE_MUTEX_INITIALIZER;
	int result;
	int tid = GetCurrentThreadId();
	
	result = pthread_mutex_lock(&mutex);
	if(result)
		return -1;

	if(tid != mutex._owner)
		return -2;

	if(1 != mutex._count)
		return -3;

	if(1 != mutex._mlock)
		return -4;
	
	return pthread_mutex_unlock(&mutex);
}

int mutex_0305(void)
{
	pthread_mutex_t	mutex = PTHREAD_RECURSIVE_MUTEX_INITIALIZER;
	int result;
	int tid = GetCurrentThreadId();
	
	result = pthread_mutex_trylock(&mutex);
	if(result)
		return -1;

	if(tid != mutex._owner)
		return -2;

	if(1 != mutex._count)
		return -3;

	if(1 != mutex._mlock)
		return -4;
	
	return pthread_mutex_unlock(&mutex);
}

int mutex_0306(void)
{
	pthread_mutex_t	mutex = PTHREAD_RECURSIVE_MUTEX_INITIALIZER;
	int result;
	int tid = GetCurrentThreadId();
	
	result = pthread_mutex_timedlock(&mutex, NULL);
	if(result)
		return -1;

	if(tid != mutex._owner)
		return -2;

	if(1 != mutex._count)
		return -3;

	if(1 != mutex._mlock)
		return -4;
	
	return pthread_mutex_unlock(&mutex);
}

/**
 *	PTHREAD_ERRORCHECK_MUTEX_INITIALIZER
 */
int mutex_0307(void)
{
	pthread_mutex_t	mutex = PTHREAD_ERRORCHECK_MUTEX_INITIALIZER;
	int result;
	int tid = GetCurrentThreadId();
	
	result = pthread_mutex_lock(&mutex);
	if(result)
		return -1;

	if(tid != mutex._owner)
		return -2;

	if(1 != mutex._mlock)
		return -4;
	
	return pthread_mutex_unlock(&mutex);
}

int mutex_0308(void)
{
	pthread_mutex_t	mutex = PTHREAD_ERRORCHECK_MUTEX_INITIALIZER;
	int result;
	int tid = GetCurrentThreadId();
	
	result = pthread_mutex_trylock(&mutex);
	if(result)
		return -1;

	if(tid != mutex._owner)
		return -2;

	if(1 != mutex._mlock)
		return -4;
	
	return pthread_mutex_unlock(&mutex);
}

int mutex_0309(void)
{
	pthread_mutex_t	mutex = PTHREAD_ERRORCHECK_MUTEX_INITIALIZER;
	int result;
	int tid = GetCurrentThreadId();
	
	result = pthread_mutex_timedlock(&mutex, NULL);
	if(result)
		return -1;

	if(tid != mutex._owner)
		return -2;

	if(1 != mutex._mlock)
		return -4;
	
	return pthread_mutex_unlock(&mutex);
}
