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

struct test_fn_t {
	const char * name_fn;
	int (*test_fn)(void);
} test_fns[] = 
{
	DECL_TEST_FN(do_test0101),
	DECL_TEST_FN(do_test0102),
	DECL_TEST_FN(do_test0201),
};

static volatile HANDLE _bgn_evt;
static volatile HANDLE _end_evt;
static volatile	int	_draw_running = 0;

void _draw_thread(void * args)
{
	int i;
	UNREFERENCED_PARAMETER(args);

	for(;;)
	{
		WaitForSingleObject(_bgn_evt, INFINITE);

		for(i = 0; _draw_running ;i++)
		{
			switch(i%4)
			{
			case 0: printf("-\b");break;
			case 1: printf("\\\b");break;
			case 2: printf("|\b");break;
			case 3: printf("/\b");break;
			}
			Sleep(1000);
		}

		SetEvent(_end_evt);
	}
}

int main()
{
	int i;
	int result;
	WORD color;
	const char * tipinfo;

	_beginthread(_draw_thread, 0, NULL);
	_bgn_evt = CreateEvent(
		NULL, FALSE, FALSE, NULL);
	_end_evt = CreateEvent(
		NULL, FALSE, FALSE, NULL);

	for(i = 0; i < sizeof(test_fns)/
		sizeof(struct test_fn_t); i++)
	{
		SetConsoleTextAttribute(
			GetStdHandle(STD_OUTPUT_HANDLE), COLOR_WHITE);
		printf("fn: %11s testing ...", test_fns[i].name_fn);

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

		printf("\b\b\b\b\b\b\b %s code: %d\n", tipinfo, result);
	}

	SetConsoleTextAttribute(
		GetStdHandle(STD_OUTPUT_HANDLE), COLOR_WHITE);
}
