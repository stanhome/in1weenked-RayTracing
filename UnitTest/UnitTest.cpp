// UnitTest.cpp : 定义控制台应用程序的入口点。
//

#include <windows.h>

#include "stdafx.h"
#include "miniUnitTest.h"
#include "AABBTest.h"

int tests_run = 0;
int tests_successed = 0;

static void log_test(const char *msg)
{
	OutputDebugStringA(msg);
	OutputDebugStringA("\n");

	printf("%s\n", msg);
}

static char *t_zero()
{
	mu_assert("this should be true.", true);

	return 0;
}

static void doAllTest()
{
	mu_run_test(t_zero);

	mu_run_test(t_AABB);
}


int main()
{
	log_test("[NOTE] --------------- BEGIN UNIT TEST ---------------\n");
	setLogFunc(log_test);

	doAllTest();

	mu_testResult;

	getchar();
    return 0;
}

