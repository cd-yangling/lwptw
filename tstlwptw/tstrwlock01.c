#include "pthread.h"
#include <errno.h>
#include <limits.h>

int do_test0101(void)
{
	unsigned int i;
	int result, except;
	pthread_rwlockattr_t attr;
	int pref;
	
	for(i = 0; ; i++)
	{
		switch(i)
		{
		case PTHREAD_RWLOCK_PREFER_READER_NP:
		case PTHREAD_RWLOCK_PREFER_WRITER_NP:
		case PTHREAD_RWLOCK_PREFER_WRITER_NONRECURSIVE_NP:
			except = 0;
			break;
		default:
			except = EINVAL;
			break;
		}
		result = pthread_rwlockattr_setkind_np(&attr, i);
		if(result != except)
			return -1;

		if(result == 0)
		{
			result = pthread_rwlockattr_getkind_np(&attr, &pref);
			if(result)
				return -2;
			if((unsigned int)pref != i)
			{
				return -3;
			}
		}

		if(i == UINT_MAX)
			break;
	}

	return 0;
}

int do_test0102(void)
{
	int result;
	pthread_rwlockattr_t attr;

	result = pthread_rwlockattr_init(&attr);
	if(result)
		return -1;

	if(attr._kind != PTHREAD_RWLOCK_DEFAULT_NP)
		return -2;

	if(attr._kind != PTHREAD_RWLOCK_PREFER_READER_NP)
		return -3;

	return pthread_rwlock_destroy(&attr);
}
