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

class ImageTexture : public Texture {
public:
	unsigned char *data;
	int nx, ny;

public:
	ImageTexture(){}
	ImageTexture(unsigned char *pixels, int width, int height)
		:data(pixels), nx(width), ny(height) {}
	virtual vec3 val(float u, float v, const vec3 &pos) const;
};

const int IMG_CHANEL = 3;

vec3 ImageTexture::val(float u, float v, const vec3 &pos) const {
	int i = u * nx;
	int j = (1 - v) * ny - 0.001;
	if (i < 0) i = 0;
	if (j < 0) j = 0;
	if (i > nx - 1) i = nx - 1;
	if (j > ny - 1) j = ny - 1;
	
	int idx = IMG_CHANEL * i + IMG_CHANEL*nx * j;
	float r = int(data[idx + 0]) / 255.0;
	float g = int(data[idx + 1]) / 255.0;
	float b = int(data[idx + 2]) / 255.0;

	return vec3(r, g, b);
}

