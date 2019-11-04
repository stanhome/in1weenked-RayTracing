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


class Translate : public Hitable {
public:
	Hitable *ptr;
	vec3 offset;

public:
	Translate(Hitable *p, const vec3 &movedBy) : ptr(p), offset(movedBy) {};
	virtual bool hit(const Ray &r, float tMin, float tMax, HitRecord &rec) const override;
	virtual bool boundingBox(float time0, float time1, AABB &box) const override;
};

bool Translate::hit(const Ray &r, float tMin, float tMax, HitRecord &rec) const {
	Ray movedR(r.origin() - offset, r.direction(), r.time());
	if (ptr->hit(movedR, tMin, tMax, rec))
	{
		rec.p += offset;
		return true;
	}

	return false;
}

bool Translate::boundingBox(float time0, float time1, AABB &box) const {
	if (ptr->boundingBox(time0, time1, box))
	{
		box = AABB(box.minPos() + offset, box.maxPos() + offset);
		return true;
	}

	return false;
}


class RotateY : public Hitable {
public:
	Hitable *ptr;
	float sinTheta;
	float cosTheta;
	bool hasBox;
	AABB bbox;

public:
	RotateY(Hitable *p, float angle);
	virtual bool hit(const Ray &r, float tMin, float tMax, HitRecord &rec) const override;
	virtual bool boundingBox(float time0, float time1, AABB &box) const override {
		box = bbox;
		return hasBox;
	}
};

RotateY::RotateY(Hitable *p, float angle) : ptr(p) {
	float radians = (M_PI / 180.0) * angle;
	sinTheta = sin(radians);
	cosTheta = cos(radians);

	hasBox = ptr->boundingBox(0, 1, bbox);
	vec3 minPos(FLT_MAX, FLT_MAX, FLT_MAX);
	vec3 maxPos(-FLT_MIN, -FLT_MIN, -FLT_MIN);
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			for (int k = 0; k < 2; k++)
			{
				// AABB ±éÀú³¤·½ÌåµÄ¶¥µã
				float x = i * bbox.maxPos().x + (1 - i) * bbox.minPos().x;
				float y = j * bbox.maxPos().y + (1 - j) * bbox.minPos().y;
				float z = k * bbox.maxPos().z + (1 - k) * bbox.minPos().z;
				float newX = cosTheta * x + sinTheta * z;
				float newZ = -sinTheta * x + cosTheta * z;
				vec3 tmp(newX, y, newZ);
				
				if (tmp.x > maxPos.x) {
					maxPos.x = tmp.x;
				}
				else if (tmp.x < minPos.x) {
					minPos.x = tmp.x;
				}

				if (tmp.y > maxPos.y) {
					maxPos.y = tmp.y;
				}
				else if (tmp.y < minPos.y) {
					minPos.y = tmp.y;
				}

				if (tmp.z > maxPos.z) {
					maxPos.z = tmp.z;
				}
				else if (tmp.z < minPos.z) {
					minPos.z = tmp.z;
				}
			}
		}
	}

	bbox = AABB(minPos, maxPos);
}

bool RotateY::hit(const Ray &r, float tMin, float tMax, HitRecord &rec) const {
	vec3 origin = r.origin();
	vec3 direction = r.direction();
	
	origin.x = cosTheta * r.origin().x - sinTheta * r.origin().z;
	origin.z = sinTheta * r.origin().x + cosTheta * r.origin().z;
	direction.x = cosTheta * r.direction().x - sinTheta * r.direction().z;
	direction.z = sinTheta * r.direction().x + cosTheta * r.direction().z;
	Ray rotatedR(origin, direction, r.time());

	if (ptr->hit(rotatedR, tMin, tMax, rec))
	{
		vec3 p = rec.p;
		p.x = cosTheta * rec.p.x + sinTheta * rec.p.z;
		p.z = -sinTheta * rec.p.x + cosTheta * rec.p.z;
		
		vec3 normal = rec.normal;
		normal.x = cosTheta * rec.normal.x + sinTheta * rec.normal.z;
		normal.z = -sinTheta * rec.normal.x + cosTheta * rec.normal.z;
		rec.p = p;
		rec.normal = normal;

		return true;
	}

	return false;
}