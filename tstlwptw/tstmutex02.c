#include "pthread.h"
#include "tstlwptw.h"
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>

int mutex_0201(void)
{
	unsigned int i;
	int result;
	pthread_mutexattr_t attr;
	pthread_mutex_t		mutex;
	
	_total_cnt = UINT_MAX;

	result = pthread_mutex_init(&mutex, NULL);
	if(result)
		return -1;

	if(PTHREAD_MUTEX_DEFAULT != mutex._flags)
		return -2;
	
	for(i = 0; ; i++)
	{
		attr._kind = i;
		result = pthread_mutex_init(&mutex, &attr);
		if(result)
			return -3;

		switch(i)
		{
		case PTHREAD_MUTEX_NORMAL:
		case PTHREAD_MUTEX_RECURSIVE:
		case PTHREAD_MUTEX_ERRORCHECK:
			if(mutex._flags != attr._kind)
				return -4;
			break;
		default:
			if(PTHREAD_MUTEX_DEFAULT != mutex._flags)
				return -5;
		}

		_cur_finish = i;
		
		if(i == UINT_MAX)
			break;
	}
	
	return pthread_mutex_destroy(&mutex);
}

int mutex_0202(void)
{
	pthread_mutex_t	m1 = PTHREAD_MUTEX_INITIALIZER;
	pthread_mutex_t	m2 = PTHREAD_RECURSIVE_MUTEX_INITIALIZER;
	pthread_mutex_t	m3 = PTHREAD_ERRORCHECK_MUTEX_INITIALIZER;
	pthread_mutex_t	m22 = PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP;
	pthread_mutex_t	m33 = PTHREAD_ERRORCHECK_MUTEX_INITIALIZER_NP;

	pthread_mutex_t e1;
	pthread_mutex_t e2;
	pthread_mutex_t e3;

	memset(&e1, 0, sizeof(e1));
	memset(&e2, 0, sizeof(e2));
	memset(&e3, 0, sizeof(e3));

	e1._flags = PTHREAD_MUTEX_DEFAULT;
	e2._flags = PTHREAD_MUTEX_RECURSIVE;
	e3._flags = PTHREAD_MUTEX_ERRORCHECK;

	if(memcmp(&e1, &m1, sizeof(pthread_mutex_t)))
		return -1;

	if(memcmp(&e2, &m2, sizeof(pthread_mutex_t)))
		return -3;

	if(memcmp(&e3, &m3, sizeof(pthread_mutex_t)))
		return -3;

	if(memcmp(&e2, &m22, sizeof(pthread_mutex_t)))
		return -4;
	
	if(memcmp(&e3, &m33, sizeof(pthread_mutex_t)))
		return -5;


	return 0;
}
