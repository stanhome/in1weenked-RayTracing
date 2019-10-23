#pragma once
#include <random>
#include <chrono>

#include "vec3.h"

std::default_random_engine s_randGenerator;

void initUtils() {
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	s_randGenerator = std::default_random_engine(seed);
}

float randCanonical() {
	return std::generate_canonical<float, std::numeric_limits<float>::digits>(s_randGenerator);
}

//////////////////////////////////////////////////////////////////////////
// geometry math

vec3 randomInUnitSphere() {
	vec3 p;
	do
	{
		p = 2.0 * vec3(randCanonical(), randCanonical(), randCanonical()) - vec3(1, 1, 1);
	} while (vec3::dot(p, p) >= 1.0);

	return p;
}

vec3 reflect(const vec3 &v, const vec3 &n) {
	return v - 2 * vec3::dot(v, n) * n;
}
