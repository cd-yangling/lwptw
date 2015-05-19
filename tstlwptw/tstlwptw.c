#include "./tstlwptw.h"
#include <stdio.h>
#include <windows.h>
#include <process.h>

#define	COLOR_WHITE				\
	(FOREGROUND_RED |			\
	FOREGROUND_GREEN |			\
	FOREGROUND_BLUE)

#define	COLOR_YELLOW			\
	(FOREGROUND_RED |			\
	FOREGROUND_GREEN)

#define	COLOR_RED				\
	(FOREGROUND_RED)

#define	DECL_TEST_FN(_name_)	\
	{							\
		#_name_,	_name_		\
	}

extern int do_test0101(void);
extern int do_test0102(void);
extern int do_test0201(void);
extern int do_test0301(void);
extern int do_test0302(void);
extern int do_test0303(void);
extern int do_test0304(void);
extern int do_test0305(void);
extern int do_test0306(void);
extern int do_test0307(void);
extern int do_test0401(void);
extern int do_test0402(void);
extern int do_test0403(void);
extern int do_test0404(void);
extern int do_test0405(void);

static struct test_fn_t {
	const char * name_fn;
	int (*test_fn)(void);
} test_fns[] = 
{
	DECL_TEST_FN(do_test0101),
	DECL_TEST_FN(do_test0102),
	DECL_TEST_FN(do_test0201),
	DECL_TEST_FN(do_test0301),
	DECL_TEST_FN(do_test0302),
	DECL_TEST_FN(do_test0303),
	DECL_TEST_FN(do_test0304),
	DECL_TEST_FN(do_test0305),
	DECL_TEST_FN(do_test0306),
	DECL_TEST_FN(do_test0307),
	DECL_TEST_FN(do_test0401),
	DECL_TEST_FN(do_test0402),
	DECL_TEST_FN(do_test0403),
	DECL_TEST_FN(do_test0404),
	DECL_TEST_FN(do_test0405),
};

static volatile HANDLE _bgn_evt;
static volatile HANDLE _end_evt;
static volatile	int	_draw_running = 0;

volatile __int64 _total_cnt;
volatile __int64 _cur_finish;

static void _draw_thread(void * args)
{
	int i;
	double	v;

	UNREFERENCED_PARAMETER(args);

	for(;;)
	{
		WaitForSingleObject(_bgn_evt, INFINITE);

		for(i = 0; _draw_running ;i++)
		{
			v = _cur_finish / (double)_total_cnt;
			v = v * 100;

			switch(i%4)
			{
			case 0: printf("%6.2f%% -\b\b\b\b\b\b\b\b\b", v);break;
			case 1: printf("%6.2f%% \\\b\b\b\b\b\b\b\b\b", v);break;
			case 2: printf("%6.2f%% |\b\b\b\b\b\b\b\b\b", v);break;
			case 3: printf("%6.2f%% /\b\b\b\b\b\b\b\b\b",v );break;
			}			
			Sleep(1000);
		}

		SetEvent(_end_evt);
	}
}

static int compare(const void *arg1, const void *arg2)
{
	return strcmp(*(char**)arg1, *(char**)arg2);
}

static char **_argv  = NULL;
static int _argc = 0;

static int lookup(const char *name_fn)
{
	void * result;

	if(NULL == _argv)
		return 1;

	result =  bsearch(&name_fn, (void *)_argv,
		(size_t)_argc, sizeof(char *), compare);

	return ((result) ? 1 : 0);
	
}

int main(int argc, char **argv)
{
	int i;
	int result;
	WORD color;
	const char * tipinfo;

	/*	Eliminate argv[0] from sort*/
	if(argc >= 2)
	{
		argv++;
		argc--;
		_argv = argv;
		_argc = argc;
		
		qsort((void *)argv, (size_t)argc, sizeof(char *), compare);
	}
	else
	{
		_argv = NULL; _argc = 0;
	}

	_beginthread(_draw_thread, 0, NULL);
	_bgn_evt = CreateEvent(
		NULL, FALSE, FALSE, NULL);
	_end_evt = CreateEvent(
		NULL, FALSE, FALSE, NULL);

	for(i = 0; i < sizeof(test_fns)/
		sizeof(struct test_fn_t); i++)
	{
		if(!lookup(test_fns[i].name_fn))
			continue;

		SetConsoleTextAttribute(
			GetStdHandle(STD_OUTPUT_HANDLE), COLOR_WHITE);
		printf("fn: %11s testing ...", test_fns[i].name_fn);

		_total_cnt	  = 1;
		_cur_finish   = 0;
		_draw_running = 1;
		SetEvent(_bgn_evt);
		if(NULL != test_fns[i].test_fn)
		{
			result = test_fns[i].test_fn();
			if(result)
			{
				color = COLOR_RED;
				tipinfo = "failed";
			}
			else
			{
				color = COLOR_WHITE;
				tipinfo = "passed";
			}
		}
		else
		{
			result= 0;
			color = COLOR_YELLOW;
			tipinfo = "ignore";
			
		}
		_draw_running = 0;
		WaitForSingleObject(_end_evt, INFINITE);

		SetConsoleTextAttribute(
				GetStdHandle(STD_OUTPUT_HANDLE), color);

		printf("\b\b\b\b\b\b\b %s code: %d    \n", tipinfo, result);
	}

	SetConsoleTextAttribute(
		GetStdHandle(STD_OUTPUT_HANDLE), COLOR_WHITE);
}
