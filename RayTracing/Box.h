#pragma once

#include "Hitable.h"
#include "HitableList.h"
#include "PlanRect.h"

class Box : public Hitable {
public:
	vec3 pmin, pmax;
	Hitable *listPtr;

public:
	Box() {}
	Box(const vec3 &p0, const vec3 &p1, Material *mat);
	virtual bool hit(const Ray &r, float tMin, float tMax, HitRecord &rec) const override;
	virtual bool boundingBox(float time0, float time1, AABB &box) const override {
		box = AABB(pmin, pmax);
		return true;
	}
};

Box::Box(const vec3 &p0, const vec3 &p1, Material *mat) : pmin(p0), pmax(p1) {
	Hitable **list = new Hitable*[6];

	// front
	list[0] = new XYRect(p0.x, p1.x, p0.y, p1.y, p1.z, mat);
	//background
	list[1] = new FlipNormals(new XYRect(p0.x, p1.x, p0.y, p1.y, p0.z, mat));
	// top
	list[2] = new XZRect(p0.x, p1.x, p0.z, p1.z, p1.y, mat);
	// bottom
	list[3] = new FlipNormals(new XZRect(p0.x, p1.x, p0.z, p1.z, p0.y, mat));

	list[4] = new YZRect(p0.y, p1.y, p0.z, p1.z, p1.x, mat);
	list[5] = new FlipNormals(new YZRect(p0.y, p1.y, p0.z, p1.z, p0.x, mat));

	listPtr = new HitableList(list, 6);
}

bool Box::hit(const Ray &r, float tMin, float tMax, HitRecord &rec) const {
	return listPtr->hit(r, tMin,tMax, rec);
}
