/*
 * @Date: 2019-10-31 15:42:42
 * @LastEditors: Stan
 * @LastEditTime: 2019-10-31 19:10:48
 */
#pragma once

#include "vec3.h"
#include "ray.h"

// ffmin() is quite a bit faster than built-in fmax(). because it donesn't worry about NaNs and other exceptions;
inline float ffmin(float a, float b) { return a < b ? a : b; }
inline float ffmax(float a, float b) { return a > b ? a : b; }

class AABB {
private:
	vec3 _min;
	vec3 _max;

public:
	const vec3 &minPos() const { return _min; }
	const vec3 &maxPos() const { return _max; }

public:
	AABB() {}
	AABB(const vec3 &min, const vec3 &max) : _min(min), _max(max) {};

	bool hit(const Ray &r, float tmin, float tmax) const {
		// x-axis
		float invD = 1.0 / r.direction().x;
		float t0 = (minPos().x - r.origin().x) * invD;
		float t1 = (maxPos().x - r.origin().x) * invD;

		auto isTMaxLessAndEqualsThanMinFunc = [&]() -> bool {
			if (invD < 0.0f) std::swap(t0, t1);
			
			tmin = t0 > tmin ? t0 : tmin;
			tmax = t1 < tmax ? t1 : tmax;

			return tmax <= tmin;
		};

		if (isTMaxLessAndEqualsThanMinFunc()) return false;

		// y-axis
		invD = 1.0 / r.direction().y;
		t0 = (minPos().y - r.origin().y) * invD;
		t1 = (maxPos().y - r.origin().y) * invD;
		if (isTMaxLessAndEqualsThanMinFunc()) return false;

		// z-axis
		invD = 1.0 / r.direction().z;
		t0 = (minPos().z - r.origin().z) * invD;
		t1 = (maxPos().z - r.origin().z) * invD;
		if (isTMaxLessAndEqualsThanMinFunc()) return false;

		return true;
	}
}; 

AABB surroundingBox(const AABB &box0, const AABB &box1) {
	vec3 minPos(ffmin(box0.minPos().x, box1.minPos().x),
				ffmin(box0.minPos().y, box1.minPos().y),
				ffmin(box0.minPos().z, box1.minPos().z));
	vec3 maxPos(ffmax(box0.maxPos().x, box1.maxPos().x),
				ffmax(box0.maxPos().y, box1.maxPos().y),
				ffmax(box0.maxPos().z, box1.maxPos().z));

	return AABB(minPos, maxPos);
}




