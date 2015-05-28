#include "pthread.h"
#include "tstlwptw.h"
#include <stdio.h>
#include <string.h>
#include <time.h>

int cond_0101(void)
{
	pthread_cond_t	cond = PTHREAD_COND_INITIALIZER;
	pthread_cond_t	temp;
	int result;

	result = pthread_cond_init(&temp, NULL);
	if(result)
		return -1;

	if(memcmp(&cond, &temp, sizeof(pthread_cond_t)))
		return -2;

	return pthread_cond_destroy(&cond);
}

int cond_0102(void)
{
	pthread_cond_t	cond = PTHREAD_COND_INITIALIZER;
	pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
	int result;
	struct timespec	abstime;

	result = pthread_mutex_lock(&mutex);
	if(result)
		return -1;

	abstime.tv_nsec = 0; abstime.tv_sec = time(NULL) + 2;
	result = pthread_cond_timedwait(&cond, &mutex, &abstime);
	if(ETIMEDOUT != result)
		return -2;

	if(1 != mutex._mlock)
		return -3;

	return pthread_cond_destroy(&cond);
}
