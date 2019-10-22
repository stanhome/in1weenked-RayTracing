#pragma once

#include "hitable.h"

class Sphere : public Hitable {
public:
	vec3 center;
	float radius;

public:
	Sphere() {};
	Sphere(vec3 cen, float r) : center(cen), radius(r) {};
	virtual bool hit(const Ray &r, float tMin, float tMax, HitRecord &rec) const;
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
			rec.t = t;
			rec.p = r.pointAtParameter(t);
			rec.normal = (rec.p - center) / radius;

			return true;
		}

		t = (-b + sqrt(discriminant)) / (2.0 * a);
		if (t > tMin && t < tMax) {
			rec.t = t;
			rec.p = r.pointAtParameter(t);
			rec.normal = (rec.p - center) / radius;

			return true;
		}
	}

	return false;
}