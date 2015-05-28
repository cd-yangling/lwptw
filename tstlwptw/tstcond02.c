#include "pthread.h"
#include "tstlwptw.h"
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <limits.h>
#include <process.h>
#include <windows.h>

/**
 *	ref UNPv2 
 *
 *	8.4 使用条件变量+互斥变量实现读写锁
 */
typedef struct my_rwlock_s
{
	pthread_mutex_t	mutex;
	pthread_cond_t	cv_rd;
	pthread_cond_t	cv_wr;
	unsigned int	nwaitreaders;
	unsigned int	nwaitwriters;
	int				rw_refcount;
	/*	-1 if writer has the lock else
	 *	readers holding the lock*/
} my_rwlock_t;

void my_rwlock_init(my_rwlock_t * rwlock)
{
	pthread_mutex_init(&rwlock->mutex, NULL);
	pthread_cond_init(&(rwlock->cv_rd), NULL);
	pthread_cond_init(&(rwlock->cv_wr), NULL);
	rwlock->nwaitreaders = 0;
	rwlock->nwaitwriters = 0;
	rwlock->rw_refcount = 0;
}

void my_rwlock_rdlock(my_rwlock_t * rwlock)
{
	int result = 0;

	if(pthread_mutex_lock(&(rwlock->mutex)))
		Sleep(INFINITE);

	/*	writer prefer*/
	while(rwlock->rw_refcount < 0 || rwlock->nwaitwriters)
	{
		++rwlock->nwaitreaders;
		result = pthread_cond_wait(
					&(rwlock->cv_rd), &(rwlock->mutex));
		--rwlock->nwaitreaders;
		if(result)
			Sleep(INFINITE);
	}

	if(result == 0)
		rwlock->rw_refcount++;

	if(pthread_mutex_unlock(&(rwlock->mutex)))
		Sleep(INFINITE);
}

void my_rwlock_wrlock(my_rwlock_t * rwlock)
{
	int result = 0;
	
	if(pthread_mutex_lock(&(rwlock->mutex)))
		Sleep(INFINITE);

	while(rwlock->rw_refcount != 0)
	{
		++rwlock->nwaitwriters;
		result = pthread_cond_wait(
					&(rwlock->cv_wr), &(rwlock->mutex));
		--rwlock->nwaitwriters;
		if(result)
			Sleep(INFINITE);
	}

	if(result == 0)
		rwlock->rw_refcount = -1;

	if(pthread_mutex_unlock(&(rwlock->mutex)))
		Sleep(INFINITE);
}

void my_rwlock_unlock(my_rwlock_t * rwlock)
{
	int result = 0;
	
	if(pthread_mutex_lock(&(rwlock->mutex)))
		Sleep(INFINITE);

	if(rwlock->rw_refcount > 0)
		--rwlock->rw_refcount;
	else if(rwlock->rw_refcount == -1)
		rwlock->rw_refcount = 0;
	else
		Sleep(INFINITE);

	if(rwlock->nwaitwriters)
	{
		if(rwlock->rw_refcount == 0)
			result = pthread_cond_signal(&(rwlock->cv_wr));
	}
	else if(rwlock->nwaitreaders)
		result = pthread_cond_broadcast(&(rwlock->cv_rd));

	if(result)
		Sleep(INFINITE);

	if(pthread_mutex_unlock(&(rwlock->mutex)))
		Sleep(INFINITE);
}

static volatile	int		_0201_bgn = 0;
static volatile	int		_0201_end = 0;
static unsigned __int64	_0201_cnt = UINT_MAX/256;

static
void atomic_inc(int * augend)
{
	__asm
	{
		mov			edx, dword ptr [augend];
		lock inc	dword ptr [edx];
	}
}

static void _0201_thread(void * args)
{
	my_rwlock_t * rw = (my_rwlock_t*)args;
	unsigned __int64	i;

	while(_0201_bgn == 0)
		Sleep(1);

	for(i = 0; i < _0201_cnt; i++)
	{
		my_rwlock_wrlock(rw);
		_cur_finish++;
		my_rwlock_unlock(rw);
		my_rwlock_rdlock(rw);
		my_rwlock_unlock(rw);
	}

	atomic_inc((int *)(&_0201_end));
}

int cond_0201(void)
{
	int i;
	my_rwlock_t rw;
	const int nr_thread = 4;

	_total_cnt = _0201_cnt * nr_thread;
	_cur_finish= 0;
	_0201_bgn = 0;
	_0201_end = 0;


	my_rwlock_init(&rw);

	for(i = 0; i < nr_thread; i++)
	{
		_beginthread(_0201_thread, 0, &rw);
	}

	_0201_bgn = 1;

	while(nr_thread != _0201_end)
		Sleep(1000);

	if(_cur_finish != _total_cnt)
		return -1;

	return 0;
}
