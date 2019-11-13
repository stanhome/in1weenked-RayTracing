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
}


void integral0() {
	const int N = 1000000; //100w
	float sum = 0;
	for (int i = 0; i < N; ++i)
	{
		float x = 2 * randCanonical();
		sum += x * x;
	}

	std::cout << "I = " << 2 * sum / N << std::endl;
}

inline float pdf(float x) {
	//return 0.5 * x;
	//return 0.5;
	return 3 * x * x / 8;
}

void integral() {
	//const int N = 1000000; //100w
	const int N = 1; //100w
	float sum = 0;
	for (int i = 0; i < N; ++i)
	{
		//float x = sqrt(4 * randCanonical());
		//float x = 2 * randCanonical();
		float x = pow(8 * randCanonical(), 1 / 3);
		sum += x * x / pdf(x);
	}

	std::cout << "I = " << sum / N << std::endl;
}

inline float pdfVec(const vec3 &p)
{
	return 1 / ( 4 * M_PI);
}

void integralPhere() {
	const int N = 1000000; //100w
	float sum = 0;
	for (int i = 0; i < N; ++i)
	{
		vec3 d = randomOnUnitSphere();
		float consineSquared = d.z * d.z;
		sum += consineSquared / pdfVec(d);
	}

	std::cout << "I = " << sum / N << std::endl;
}
//I = 4.18885 即 4π/3



int main()
{
	initUtils();
	//estimatePi();
	//estimatePi2();
	//estimatePi3();

	//integral0();
	//integral();

	integralPhere();

	getchar();
}

