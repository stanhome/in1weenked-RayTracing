#pragma once

#include "ONB.h"
#include "utils.h"

class PDF {
public:
	virtual float val(const vec3 direction) const = 0;
	virtual vec3 generate() const = 0;
};


class CosinePdf : public PDF {
public:
	ONB uvw;

public:
	CosinePdf(const vec3 &w) { uvw.buildFromW(w); }

	virtual float val(const vec3 direction) const override {
		float cosine = vec3::dot(direction.normalized(), uvw.w());
		if (cosine)
		{
			return cosine / M_PI;
		}
		else {
			return 0;
		}
	}

	virtual vec3 generate() const override {
		return uvw.local(randomCosineDirection());
	}
};


class HittablePdf : public PDF {
public:
	vec3 o;
	Hittable *ptr;

public:
	HittablePdf(Hittable *p, const vec3& origin) : ptr(p), o(origin) {}

	virtual float val(const vec3 direction) const override {
		return ptr->pdfVal(o, direction);
	}

	virtual vec3 generate() const override {
		return ptr->random(o);
	}
};


class MixturePdf : public PDF {
public:
	PDF *p[2];

public:
	MixturePdf(PDF *p0, PDF *p1) { p[0] = p0, p[1] = p1; }
	virtual float val(const vec3 direction) const override {
		return 0.5 * p[0]->val(direction) + 0.5 * p[1]->val(direction);
	}

	virtual vec3 generate() const override {
		if (randCanonical() < 0.5)
		{
			return p[0]->generate();
		}
		else
		{
			return p[1]->generate();
		}
	}
};