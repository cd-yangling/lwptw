#include "pthread.h"
#include "tstlwptw.h"
#include <errno.h>
#include <limits.h>
#include <string.h>

int do_test0201(void)
{
	unsigned int i;
	int result;
	pthread_rwlock_t	reader_p;
	pthread_rwlock_t	writer_p;
	pthread_rwlock_t	rwlock_v;
	pthread_rwlockattr_t	attr;

	_total_cnt = UINT_MAX;

	memset(&reader_p, 0, sizeof(reader_p));
	memset(&writer_p, 0, sizeof(writer_p));
	writer_p._flags = 1;

	result = pthread_rwlock_init(&rwlock_v, NULL);
	if(result)
		return -1;

	if(memcmp(&rwlock_v, &reader_p,
		sizeof(pthread_rwlock_t)))
		return -2;

	result = pthread_rwlockattr_init(&attr);
	if(result)
		return -3;

	result = pthread_rwlock_init(&rwlock_v, &attr);
	if(result)
		return -4;

	if(memcmp(&rwlock_v, &reader_p,
		sizeof(pthread_rwlock_t)))
		return -5;

	for(i = 0; ;i++)
	{
		switch(i)
		{
		case PTHREAD_RWLOCK_PREFER_READER_NP:
		case PTHREAD_RWLOCK_PREFER_WRITER_NP:
		case PTHREAD_RWLOCK_PREFER_WRITER_NONRECURSIVE_NP:
			{
				result = pthread_rwlockattr_setkind_np(&attr, i);
				if(result)
					return -6;

				result = pthread_rwlock_init(&rwlock_v, &attr);
				if(result)
					return -7;

				if(PTHREAD_RWLOCK_PREFER_WRITER_NONRECURSIVE_NP == i)
				{
					if(memcmp(&rwlock_v, &writer_p,
						sizeof(pthread_rwlock_t)))
						return -8;
				}
				else
				{
					if(memcmp(&rwlock_v, &reader_p,
						sizeof(pthread_rwlock_t)))
						return -9;
				}
			}
			break;
		default:
			break;
		}

		_cur_finish = i;

		if(i == UINT_MAX)
		{
			break;
		}
	}

	return pthread_rwlock_destroy(&rwlock_v);
}