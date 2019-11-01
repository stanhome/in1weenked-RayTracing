#pragma once

#include "vec3.h"
#include "utils.h"

inline float PerlinInterpolate(vec3 c[2][2][2], float u, float v, float w) {
	float uu = u * u * (3 - 2 * u);
	float vv = v * v * (3 - 2 * v);
	float ww = w * w * (3 - 2 * w);
	float accum = 0;
	
	for (int i = 0; i < 2; ++i)
	{
		for (int j = 0; j < 2; ++j)
		{
			for (int k = 0; k < 2; ++k)
			{
				vec3 weightV(u - i, v - j, w - k);
				accum += (i * uu + (1 - i) *(1 - uu))
					* (j * vv + (1 - j) * (1 - vv))
					* (k * ww + (1 - k) * (1 - ww))
					* vec3::dot(c[i][j][k], weightV);
			}
		}
	}

	return accum;
}

class Perlin {
public:
	static vec3 *s_ranVec;
	static int *s_permX;
	static int *s_permY;
	static int *s_permZ;

public:
	float noise(const vec3 &pos) const {
		float u = pos.x - floor(pos.x);
		float v = pos.y - floor(pos.y);
		float w = pos.z - floor(pos.z);
		int i = floor(pos.x);
		int j = floor(pos.y);
		int k = floor(pos.z);

		vec3 c[2][2][2];
		for (int di = 0; di < 2; ++di)
		{
			for (int dj = 0; dj < 2; ++dj)
			{
				for (int dk = 0; dk < 2; ++dk)
				{
					c[di][dj][dk] = s_ranVec[
						s_permX[(i + di) & 255] ^ s_permY[(j + dj) & 255] ^ s_permZ[(k + dk) & 255]];
				}
			}
		}

		return PerlinInterpolate(c, u, v, w);
	}

	float turb(const vec3 &pos, int depth = 7) const {
		float accum = 0;
		vec3 tempPos = pos;
		float weight = 1.0;
		for (int i = 0; i < depth; i++)
		{
			accum += weight * noise(tempPos);
			weight *= 0.5;
			tempPos *= 2;
		}

		return fabs(accum);
	}
};

static vec3 *perlinGenerate() {
	vec3 *p = new vec3[256];
	for (int i = 0; i < 256; ++i)
	{
		p[i] = vec3(-1 + 2 * randCanonical(), -1 + 2 * randCanonical(), -1 + 2 * randCanonical()).normalized();
	}

	return p;
}

void permute(int *p, int n) {
	for (int i = n-1; i > 0; --i)
	{
		int target = int(randCanonical() * (i + 1));
		int tmp = p[i];
		p[i] = p[target];
		p[target] = tmp;
	}
}

static int *perlinGeneratePerm() {
	int *p = new int[256];

	for (int i = 0; i < 256; ++i)
	{
		p[i] = i;
	}

	permute(p, 256);

	return p;
}

vec3 *Perlin::s_ranVec = perlinGenerate();
int *Perlin::s_permX = perlinGeneratePerm();
int *Perlin::s_permY = perlinGeneratePerm();
int *Perlin::s_permZ = perlinGeneratePerm();
