#pragma once

#include "hitable.h"

class Sphere : public Hitable {
public:
	vec3 center;
	float radius;
	Material *mat;

public:
	Sphere() {};
	Sphere(vec3 cen, float r, Material *m) : center(cen), radius(r), mat(m) {};
	virtual bool hit(const Ray &r, float tMin, float tMax, HitRecord &rec) const;
	virtual bool boundingBox(float time0, float time1, AABB &box) const;

private:
	void assignHitRecord(HitRecord &rec, float t, const Ray &r) const {
		rec.t = t;
		rec.matPtr = mat;
		rec.p = r.pointAtParameter(t);
		getSphereUV((rec.p - center) / radius, rec.u, rec.v);
		rec.normal = (rec.p - center) / radius;
	}
};



bool Sphere::hit(const Ray &r, float tMin, float tMax, HitRecord &rec) const {
	vec3 oc = r.origin() - center;
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

bool Sphere::boundingBox(float time0, float time1, AABB &box) const {
	vec3 rVec(radius);
	box = AABB(center - rVec, center + rVec);

	return true;
}