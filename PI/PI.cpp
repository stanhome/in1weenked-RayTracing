// PI.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include "utils.h"


// Suppose R = 1
void estimatePi()
{
	const int N = 10000;
	int insideCircleCount = 0;

	for (int i = 0; i < N; ++i)
	{
		// x: [-1, 1), y : [-1, 1)
		float x = 2 * randCanonical() - 1;
		float y = 2 * randCanonical() - 1;
		if (x * x + y * y < 1)
		{
			insideCircleCount++;
		}
	}

	std::cout << "Estimate of PI = " << 4 * float(insideCircleCount) / N << std::endl;
	getchar();
}

void estimatePi2()
{
	int insideCircleCount = 0;
	int runs = 0;

	while (true)
	{
		++runs;
		// x: [-1, 1), y : [-1, 1)
		float x = 2 * randCanonical() - 1;
		float y = 2 * randCanonical() - 1;
		if (x * x + y * y < 1)
		{
			insideCircleCount++;
		}

		// per 10W runs
		if (runs % 100000 == 0)
		{
			std::cout << "Estimate of PI = " << 4 * float(insideCircleCount) / runs << std::endl;
		}
	}
}

void estimatePi3()
{
	const int SQRT_N = 10000;
	int insideCircleCount = 0;
	int insideCircleStratified = 0;

	for (int i = 0; i < SQRT_N; ++i)
	{
		for (int j = 0; j < SQRT_N; ++j)
		{
			// x: [-1, 1), y : [-1, 1)
			float x = 2 * randCanonical() - 1;
			float y = 2 * randCanonical() - 1;
			if (x * x + y * y < 1)
				insideCircleCount++;

			x = 2 * ((i + randCanonical()) / SQRT_N) - 1;
			y = 2 * ((j + randCanonical()) / SQRT_N) - 1;
			if (x * x + y * y < 1)
				insideCircleStratified++;
		}
	}

	std::cout << "Regular Estimate of PI = " << 4 * float(insideCircleCount) / (SQRT_N * SQRT_N) << std::endl;
	std::cout << "Stratified Estimate of PI = " << 4 * float(insideCircleStratified) / (SQRT_N * SQRT_N) << std::endl;
	getchar();
}

int main()
{
	initUtils();
	//estimatePi();
	//estimatePi2();
	estimatePi3();
}

