#include "pthread.h"
#include "tstlwptw.h"
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <process.h>
#include <windows.h>
#include <time.h>

static volatile HANDLE	_h1;
static volatile HANDLE	_h2;
static volatile	int		_0401_over = 0;
static volatile	int		_0402_over = 0;
static volatile int		_0404_over = 0;
static volatile int		_0405R_over = 0;
static volatile int		_0405W_over = 0;
static void _0401_thread(void * args)
{
	int result;
	pthread_rwlock_t * rwlock = 
		(pthread_rwlock_t*)args;

	result = pthread_rwlock_wrlock(rwlock);
	if(result)
	{
		return;
	}

	SetEvent(_h1);

	(void) WaitForSingleObject(_h2, INFINITE);

	_0401_over = 1;
}

/**
 *	held by other thread for WR
 *
 *	pthread_rwlock_trylock
 *	pthread_rwlock_timedlock
 *
 *	reader prefer case
 */
int do_test0401(void)
{
	DWORD res;
	int result;
	pthread_rwlock_t rwlock = PTHREAD_RWLOCK_INITIALIZER;
	struct timespec	abstime;

	_h1 = CreateEvent(NULL, FALSE, FALSE, NULL);
	_h2 = CreateEvent(NULL, FALSE, FALSE, NULL);
	_0401_over = 0;

	_beginthread(_0401_thread, 0, &rwlock);

	res = WaitForSingleObject(_h1, 15 * 1000);
	if(res != WAIT_OBJECT_0)
	{
		CloseHandle(_h1);
		CloseHandle(_h2);
		return -1;
	}

	result = pthread_rwlock_tryrdlock(&rwlock);
	if(result != EBUSY)
	{
		CloseHandle(_h1);
		CloseHandle(_h2);
		return -2;
	}

	abstime.tv_sec = time(NULL) + 3;
	abstime.tv_nsec= 0;

	result = pthread_rwlock_timedrdlock(&rwlock, &abstime);
	if(result != ETIMEDOUT)
		return -3;

	SetEvent(_h2);

	while(_0401_over == 0)
		Sleep(1);

	CloseHandle(_h1);
	CloseHandle(_h2);

	return 0;
}

static void _0402_thread(void * args)
{
	int result;
	pthread_rwlock_t * rwlock = 
		(pthread_rwlock_t*)args;

	result = pthread_rwlock_wrlock(rwlock);
	if(result)
		Sleep(INFINITE);

	_0402_over = 1;
}

/**
 *	reader prefer case
 */
int do_test0402(void)
{
	int result;
	pthread_rwlock_t rwlock = PTHREAD_RWLOCK_INITIALIZER;

	_0402_over = 0;
	result = pthread_rwlock_rdlock(&rwlock);
	if(result)
		return -1;

	_beginthread(_0402_thread, 0, &rwlock);

	while(rwlock._nr_writers_queued == 0)
		Sleep(1);

	result = pthread_rwlock_rdlock(&rwlock);
	if(result)
		return -2;

	result = pthread_rwlock_tryrdlock(&rwlock);
	if(result)
		return -3;

	result = pthread_rwlock_timedrdlock(&rwlock, NULL);
	if(result)
		return -4;

	result = pthread_rwlock_unlock(&rwlock);
	if(result)
		return -5;

	result = pthread_rwlock_unlock(&rwlock);
	if(result)
		return -6;

	result = pthread_rwlock_unlock(&rwlock);
	if(result)
		return -7;

	result = pthread_rwlock_unlock(&rwlock);
	if(result)
		return -8;

	while(_0402_over == 0)
		Sleep(1);

	return 0;
}


/**
 *	writer prefer case
 */
int do_test0403(void)
{
	int result;
	pthread_rwlock_t rwlock;
	pthread_rwlockattr_t attr;
	struct timespec	abstime;

	result = pthread_rwlockattr_setkind_np(
		&attr, PTHREAD_RWLOCK_PREFER_WRITER_NONRECURSIVE_NP);
	if(result)
		return -1;

	result = pthread_rwlock_init(&rwlock, &attr);
	if(result)
		return -2;

	rwlock._nr_writers_queued++;	/*DUMMY*/

	result = pthread_rwlock_tryrdlock(&rwlock);
	if(result != EBUSY)
	{
		CloseHandle(_h1);
		CloseHandle(_h2);
		return -2;
	}
	
	abstime.tv_sec = time(NULL) + 3;
	abstime.tv_nsec= 0;
	
	result = pthread_rwlock_timedrdlock(&rwlock, &abstime);
	if(result != ETIMEDOUT)
		return -3;

	return 0;
}

static void _0404_thread(void * args)
{
	int result;
	pthread_rwlock_t * rwlock = 
		(pthread_rwlock_t*)args;

	result = pthread_rwlock_rdlock(rwlock);
	if(result)
		return;

	result = pthread_rwlock_unlock(rwlock);
	if(result)
		return;

	_0404_over = 1;
}

/**
 *	0401 pthread_rwlock_rdlock case
 */
int do_test0404(void)
{
	int result;

	pthread_rwlock_t rwlock = PTHREAD_RWLOCK_INITIALIZER;

	_0404_over = 0;
	result = pthread_rwlock_wrlock(&rwlock);
	if(result)
	{
		return -1;
	}

	_beginthread(_0404_thread, 0, &rwlock);

	while(rwlock._nr_readers_queued == 0)
		Sleep(1);

	result = pthread_rwlock_unlock(&rwlock);
	if(result)
		return -2;

	while(rwlock._nr_readers_queued != 0)
		Sleep(1);

	while(_0404_over == 0)
		Sleep(1);

	return 0;
}

static void _0405R_thread(void * args)
{
	int result;
	pthread_rwlock_t * rwlock = 
		(pthread_rwlock_t*)args;
	
	result = pthread_rwlock_rdlock(rwlock);
	if(result)
		return;

	result = pthread_rwlock_unlock(rwlock);
	if(result)
		return;
	
	_0405R_over = 1;
}

static void _0405W_thread(void * args)
{
	int result;
	pthread_rwlock_t * rwlock = 
		(pthread_rwlock_t*)args;
	
	result = pthread_rwlock_wrlock(rwlock);
	if(result)
		return;

	result = pthread_rwlock_unlock(rwlock);
	if(result)
		return;

	_0405W_over = 1;
}

/**
 *	0403 pthread_rwlock_rdlock case
 */
int do_test0405(void)
{
	int result;
	
	pthread_rwlock_t rwlock;
	pthread_rwlockattr_t attr;

	_0405R_over = 0;
	_0405W_over = 0;

	result = pthread_rwlockattr_setkind_np(
		&attr, PTHREAD_RWLOCK_PREFER_WRITER_NONRECURSIVE_NP);
	if(result)
		return -1;

	result = pthread_rwlock_init(&rwlock, &attr);
	if(result)
		return -2;

	result = pthread_rwlock_wrlock(&rwlock);
	if(result)
		return -3;
	
	_beginthread(_0405W_thread, 0, &rwlock);

	while(rwlock._nr_writers_queued == 0)
		Sleep(1);

	_beginthread(_0405R_thread, 0, &rwlock);
	while(rwlock._nr_readers_queued == 0)
		Sleep(1);

	result = pthread_rwlock_unlock(&rwlock);
	if(result)
		return -4;

	while(_0405R_over == 0)
		Sleep(1);

	while(_0405W_over == 0)
		Sleep(1);

	return 0;
}
