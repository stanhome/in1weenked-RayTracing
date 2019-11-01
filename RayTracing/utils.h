#pragma once
#include <random>
#include <chrono>

#include "vec3.h"

#define M_PI       3.14159265358979323846   // pi

std::default_random_engine s_randGenerator;

void initUtils() {
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	s_randGenerator = std::default_random_engine(seed);
}

// return Floating point value in range [0, 1)
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

vec3 randomInUnitDisk() {
	vec3 p;
	do 
	{
		p = 2.0 * vec3(randCanonical(), randCanonical(), 0) - vec3(1, 1, 0);
	} while (vec3::dot(p, p) >= 1.0);

	return p;
}

// 反射
vec3 reflect(const vec3 &v, const vec3 &n) {
	return v - 2 * vec3::dot(v, n) * n;
}

//折射(斯涅耳定律)
bool refract(const vec3 &v, const vec3 &n, float niOverNt, vec3 &refracted) {
	vec3 vNormalized = v.normalized();
	float dt = vec3::dot(vNormalized, n);

	// 根据斯涅耳定律，n sin(theta) = n’sin(theta’) 推得：
	float discriminant = 1.0 - niOverNt * niOverNt * (1 - dt* dt);
	if (discriminant > 0)
	{
		// 发生折射
		refracted = niOverNt * (v + abs(dt) * n) - n * sqrt(discriminant);
		return true;
	}
	else
	{
		// 发生全反射
		return false;
	}
}

float schlick(float cosine, float refIdx) {
	float r0 = (1 - refIdx) / (1 + refIdx);
	r0 = r0 * r0;
	
	return r0 + (1 - r0) * pow((1 - cosine), 5);
}

