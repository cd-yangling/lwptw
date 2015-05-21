#include "pthread.h"
#include "tstlwptw.h"
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <process.h>
#include <time.h>
#include <windows.h>

int do_test0601(void)
{
	int result;
	pthread_rwlock_t rwlock = PTHREAD_RWLOCK_INITIALIZER;

	result = pthread_rwlock_trywrlock(&rwlock);
	if(result)
		return -1;

	result = pthread_rwlock_trywrlock(&rwlock);
	if(result != EBUSY)
		return -2;

	return 0;
}

static volatile HANDLE	_h1;
static volatile HANDLE	_h2;
static volatile	int		_0602_over = 0;
static volatile	int		_0603_over = 0;
static volatile	int		_0604_over = 0;
static void _0602_thread(void * args)
{
	int result;
	pthread_rwlock_t * rwlock = 
		(pthread_rwlock_t*)args;

	result = pthread_rwlock_rdlock(rwlock);
	if(result)
		return;

	SetEvent(_h1);

	WaitForSingleObject(_h2, INFINITE);

	_0602_over = 1;
}

int do_test0602(void)
{
	DWORD res;
	int result;
	struct timespec	abstime;
	pthread_rwlock_t rwlock = PTHREAD_RWLOCK_INITIALIZER;

	_h1 = CreateEvent(NULL, FALSE, FALSE, NULL);
	_h2 = CreateEvent(NULL, FALSE, FALSE, NULL);
	_0602_over = 0;

	_beginthread(_0602_thread, 0, &rwlock);

	res = WaitForSingleObject(_h1, 15*1000);
	if(res != WAIT_OBJECT_0)
	{
		CloseHandle(_h1);
		CloseHandle(_h2);
		return -1;
	}

	result = pthread_rwlock_trywrlock(&rwlock);
	if(result != EBUSY)
	{
		CloseHandle(_h1);
		CloseHandle(_h2);
		return -2;
	}

	abstime.tv_sec = time(NULL) + 3;
	abstime.tv_nsec= 0;
	result = pthread_rwlock_timedwrlock(&rwlock, &abstime);
	if(result != ETIMEDOUT)
	{
		CloseHandle(_h1);
		CloseHandle(_h2);
		return -3;
	}

	SetEvent(_h2);	// send signal tell 0602 thread Terminate

	while(_0602_over == 0)
		Sleep(1);

	CloseHandle(_h1);
	CloseHandle(_h2);

	return 0;
}

static void _0603_thread(void * args)
{
	int result;
	pthread_rwlock_t * rwlock = 
		(pthread_rwlock_t*)args;
	
	result = pthread_rwlock_wrlock(rwlock);
	if(result)
		return;
	
	SetEvent(_h1);

	WaitForSingleObject(_h2, INFINITE);
	
	_0603_over = 1;
}

int do_test0603(void)
{
	DWORD res;
	int result;
	struct timespec	abstime;
	pthread_rwlock_t rwlock = PTHREAD_RWLOCK_INITIALIZER;
	
	_h1 = CreateEvent(NULL, FALSE, FALSE, NULL);
	_h2 = CreateEvent(NULL, FALSE, FALSE, NULL);
	_0603_over = 0;
	
	_beginthread(_0603_thread, 0, &rwlock);
	
	res = WaitForSingleObject(_h1, 15*1000);
	if(res != WAIT_OBJECT_0)
	{
		CloseHandle(_h1);
		CloseHandle(_h2);
		return -1;
	}
	
	result = pthread_rwlock_trywrlock(&rwlock);
	if(result != EBUSY)
	{
		CloseHandle(_h1);
		CloseHandle(_h2);
		return -2;
	}
	
	abstime.tv_sec = time(NULL) + 3;
	abstime.tv_nsec= 0;
	result = pthread_rwlock_timedwrlock(&rwlock, &abstime);
	if(result != ETIMEDOUT)
	{
		CloseHandle(_h1);
		CloseHandle(_h2);
		return -3;
	}
	
	SetEvent(_h2);	// send signal tell 0602 thread Terminate
	
	while(_0603_over == 0)
		Sleep(1);
	
	return 0;
}

static void _0604_thread(void * args)
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

	_0604_over = 1;
}

int do_test0604(void)
{
	int result;
	pthread_rwlock_t rwlock = PTHREAD_RWLOCK_INITIALIZER;

	_0604_over = 0;

	result = pthread_rwlock_wrlock(&rwlock);
	if(result)
		return -1;

	_beginthread(_0604_thread, 0, &rwlock);

	while(rwlock._nr_writers_queued == 0)
		Sleep(1);

	result = pthread_rwlock_unlock(&rwlock);
	if(result)
		return -2;

	while(_0604_over == 0)
		Sleep(1);

	return 0;
}
