#pragma once

#include "ray.h"
#include "aabb.h"

class Material;

struct HitRecord {
	// 碰撞射线的步长
	float t;
	float u , v;
	// 射线与球的碰撞点
	vec3 p;
	// 碰撞点处的，球的法线（已归一化）
	vec3 normal;
	Material *matPtr;
};

class Hitable {
public:
	std::string name = "NaN";

public:
	virtual bool hit(const Ray &r, float tMin, float tMax, HitRecord &rec) const = 0;
	virtual bool boundingBox(float time0, float time1, AABB &box) const = 0;
};


class FlipNormals : public Hitable {
public:
	Hitable *ptr;

public:
	FlipNormals(Hitable *p) : ptr(p) {}
	virtual bool hit(const Ray &r, float tMin, float tMax, HitRecord &rec) const override {
		if (ptr->hit(r, tMin, tMax, rec))
		{
			rec.normal = -rec.normal;
			return true;
		}

		return false;
	}

	virtual bool boundingBox(float time0, float time1, AABB &box) const override {
		return ptr->boundingBox(time0, time1, box);
	}
};