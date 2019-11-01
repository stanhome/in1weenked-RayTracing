#pragma once

#include "vec3.h"
#include "Perlin.h"

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

class NoiseTexture : public Texture {
public:
	Perlin noise;
	float scale;

public:
	NoiseTexture() {}
	NoiseTexture(float sc) : scale(sc) {}
	virtual vec3 val(float u, float v, const vec3 &pos) const {
		//return vec3::ONE * 0.5 * (1 + noise.turb(scale * pos));
		//return vec3::ONE * noise.turb(scale * pos);
		return vec3::ONE * 0.5 * (1 + sin(scale * pos.z + 10 * noise.turb(pos))); // ¥Û¿Ì Ø
	}

};