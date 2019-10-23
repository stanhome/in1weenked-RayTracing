#pragma once

#include "ray.h"

struct HitRecord {
	// 碰撞射线的步长
	float t;
	// 射线与球的碰撞点
	vec3 p;
	// 碰撞点处的，球的法线（已归一化）
	vec3 normal;
};

class Hitable {
public:
	virtual bool hit(const Ray &r, float tMin, float tMax, HitRecord &rec) const = 0;
};