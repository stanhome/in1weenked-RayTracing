#pragma once

#include "Hittable.h"

class MovingSphere : public Hittable {
public:
	vec3 center0, center1, centerScope;
	float time0, time1, timeInterval;
	float radius;
	Material *mat;

public:
	MovingSphere() {};
	MovingSphere(const vec3 &c0, const vec3 &c1, float t0, float t1, float r, Material *m)
		: center0(c0), center1(c1), centerScope(c1 - c0)
		, time0(t0), time1(t1), radius(r), mat(m), timeInterval(t1 - t0) {}

	virtual bool hit(const Ray &r, float tMin, float tMax, HitRecord &rec) const;
	virtual bool boundingBox(float time0, float time1, AABB &box) const;
	vec3 center(float time) const;

private:
	void assignHitRecord(HitRecord &rec, float t, const Ray &r) const {
		rec.t = t;
		rec.matPtr = mat;
		rec.p = r.pointAtParameter(t);
		rec.normal = (rec.p - center(r.time())) / radius;
	}
};

vec3 MovingSphere::center(float time) const {
	return center0 + ((time - time0) / timeInterval) * centerScope;
}

bool MovingSphere::hit(const Ray &r, float tMin, float tMax, HitRecord &rec) const {
	vec3 oc = r.origin() - center(r.time());
	float a = vec3::dot(r.direction(), r.direction());
	float b = 2 * vec3::dot(r.direction(), oc);
	float c = vec3::dot(oc, oc) - radius * radius;
	float discriminant = b*b - 4 * a*c;

	if (discriminant > 0)
	{
		float t = (-b - sqrt(discriminant)) / (2.0 * a);
		if (t > tMin && t < tMax) {
			assignHitRecord(rec, t, r);

			return true;
		}

		t = (-b + sqrt(discriminant)) / (2.0 * a);
		if (t > tMin && t < tMax) {
			assignHitRecord(rec, t, r);

			return true;
		}
	}

	return false;
}

bool MovingSphere::boundingBox(float time0, float time1, AABB &box) const {
	vec3 rVec(radius);
	AABB box0(center(time0) - rVec, center(time0) + rVec);
	AABB box1(center(time1) - rVec, center(time1) + rVec);
	box = surroundingBox(box0, box1);

	return true;
}