#pragma once

#include "Hittable.h"

const float THICKNESS = 0.0001;

class XYRect : public Hittable {
public:
	Material *mat;
	float x0, x1, y0, y1, k;
	float w, h;

public:
	XYRect() {}
	XYRect(float xx0, float xx1, float yy0, float yy1, float kk, Material *material)
		: x0(xx0), x1(xx1), y0(yy0), y1(yy1), k(kk)
		, w(x1 - x0), h(y1 - y0), mat(material) {
			Hittable::name = "XYRect";
		}

	virtual bool hit(const Ray &r, float tMin, float tMax, HitRecord &rec) const override;
	virtual bool boundingBox(float time0, float time1, AABB &box) const override {
		box = AABB(vec3(x0, y0, k - THICKNESS), vec3(x1, y1, k + THICKNESS));
		return true;
	};
};

bool XYRect::hit(const Ray &r, float tMin, float tMax, HitRecord &rec) const {
	do 
	{
		float t = (k - r.origin().z) / r.direction().z;
		if (t < tMin || t > tMax) break;

		float x = r.origin().x + t * r.direction().x;
		float y = r.origin().y + t * r.direction().y;
		if (x < x0 || x > x1 || y < y0 || y > y1) break;

		rec.u = (x - x0) / w;
		rec.v = (y - y0) / h;
		rec.t = t;
		rec.matPtr = mat;
		rec.p = r.pointAtParameter(t);
		rec.normal = vec3(0, 0, 1);

		return true;
	} while (false);

	return false;
}

class XZRect : public Hittable {
public:
	Material *mat;
	float x0, x1, z0, z1, k;
	float w, h;

public:
	XZRect() {};
	XZRect(float xx0, float xx1, float zz0, float zz1, float kk, Material *material)
		: x0(xx0), x1(xx1), z0(zz0), z1(zz1), k(kk)
		, w(x1 - x0), h(z1 - z0), mat(material) {
			Hittable::name = "XZRect";
		};

	virtual bool hit(const Ray &r, float tMin, float tMax, HitRecord &rec) const override;
	virtual bool boundingBox(float time0, float time1, AABB &box) const override {
		box = AABB(vec3(x0, k-THICKNESS, z0), vec3(x1, k+THICKNESS, z1));
		return true;
	}
};

bool XZRect::hit(const Ray &r, float tMin, float tMax, HitRecord &rec) const {
	do
	{
		float t = (k - r.origin().y) / r.direction().y;
		if (t < tMin || t > tMax) break;

		float x = r.origin().x + t * r.direction().x;
		float z = r.origin().z + t * r.direction().z;
		if (x < x0 || x > x1 || z < z0 || z > z1) break;

		rec.u = (x - x0) / w;
		rec.v = (z - z0) / h;
		rec.t = t;
		rec.matPtr = mat;
		rec.p = r.pointAtParameter(t);
		rec.normal = vec3(0, 1, 0);

		return true;
	} while (false);

	return false;
}

class YZRect : public Hittable {
public:
	Material *mat;
	float y0, y1, z0, z1, k;
	float w, h;

public:
	YZRect() {};
	YZRect(float yy0, float yy1, float zz0, float zz1, float kk, Material *material)
		: y0(yy0), y1(yy1), z0(zz0), z1(zz1), k(kk)
		, w(y1 - y0), h(z1 - z0), mat(material) {};

	virtual bool hit(const Ray &r, float tMin, float tMax, HitRecord &rec) const override;
	virtual bool boundingBox(float time0, float time1, AABB &box) const override {
		box = AABB(vec3(k - THICKNESS, y0, z0), vec3(k + THICKNESS, y1, z1));
		return true;
	}
};

bool YZRect::hit(const Ray &r, float tMin, float tMax, HitRecord &rec) const {
	do
	{
		float t = (k - r.origin().x) / r.direction().x;
		if (t < tMin || t > tMax) break;

		float y = r.origin().y + t * r.direction().y;
		float z = r.origin().z + t * r.direction().z;
		if (y < y0 || y > y1 || z < z0 || z > z1) break;

		rec.u = (y - y0) / w;
		rec.v = (z - z0) / h;
		rec.t = t;
		rec.matPtr = mat;
		rec.p = r.pointAtParameter(t);
		rec.normal = vec3(1, 0, 0);

		return true;
	} while (false);

	return false;
}