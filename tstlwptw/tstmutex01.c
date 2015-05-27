#include "pthread.h"
#include "tstlwptw.h"
#include <errno.h>
#include <limits.h>

int mutex_0101(void)
{
	pthread_mutexattr_t	attr;
	int result;

	result = pthread_mutexattr_init(&attr);
	if(result)
		return -1;

	if(attr._kind != PTHREAD_MUTEX_DEFAULT)
		return -2;

	return pthread_mutexattr_destroy(&attr);
}

int mutex_0102(void)
{
	unsigned int i;
	int result, except;
	int type;
	pthread_mutexattr_t attr;

	_total_cnt = UINT_MAX;

	for(i = 0; ; i++)
	{
		result = pthread_mutexattr_settype(&attr, i);

		switch(i)
		{
		case PTHREAD_MUTEX_NORMAL:
		case PTHREAD_MUTEX_RECURSIVE:
		case PTHREAD_MUTEX_ERRORCHECK:
			except = 0;
			break;
		default:
			except = EINVAL;
		}

		if(result != except)
			return -1;

		result = pthread_mutexattr_gettype(&attr, &type);
		if(result)
			return -2;

		switch(i)
		{
		case PTHREAD_MUTEX_NORMAL:
		case PTHREAD_MUTEX_RECURSIVE:
		case PTHREAD_MUTEX_ERRORCHECK:
			if((unsigned int)type != i)
				return -3;
			break;
		default:
			break;
		}

		_cur_finish = i;
		
		if(i == UINT_MAX)
			break;
	}

	return 0;
}