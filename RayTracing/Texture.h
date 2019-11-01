#pragma once

#include "vec3.h"

class Texture {
public:
	virtual vec3 val(float u, float v, const vec3 &pos) const = 0;
};

class CheckerTexture : public Texture {
public:
	Texture *odd;
	Texture *even;

public:
	CheckerTexture() {}
	CheckerTexture(Texture *t0, Texture *t1): even(t0), odd(t1) {};

	virtual vec3 val(float u, float v, const vec3 &pos) const {
		float sines = sin(10 * pos.x) * sin(10 * pos.y) * sin(10 * pos.z);

		return sines < 0 ? odd->val(u, v, pos) : even->val(u, v, pos);
	}
};

class ConstantTexture : public Texture {
public:
	vec3 color;

public:
	ConstantTexture() {}
	ConstantTexture(const vec3 &c) : color(c) {}

	virtual vec3 val(float u, float v, const vec3 &pos) const {
		return color;
	}
};