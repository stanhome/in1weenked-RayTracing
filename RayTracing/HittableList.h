#pragma once

#include "Hittable.h"

class HittableList : public Hittable {
public:
	Hittable **list;
	int size;

public:
	HittableList() {}
	HittableList(Hittable **l, int n) : list(l), size(n) {}
	virtual bool hit(const Ray &r, float tMin, float tMax, HitRecord &rec) const;
	virtual bool boundingBox(float time0, float time1, AABB &box) const;

	float pdfVal(const vec3 &o, const vec3 &v) const override;
	vec3 random(const vec3 &o) const override;
};

bool HittableList::hit(const Ray &r, float tMin, float tMax, HitRecord &rec) const {
	HitRecord temRec;

	bool hitAnything = false;
	double closestSoFar = tMax;
	for (int i = 0; i < size; i++)
	{
		if (list[i]->hit(r, tMin, closestSoFar, temRec))
		{
			hitAnything = true;
			closestSoFar = temRec.t;
			rec = temRec;
		}
	}

	return hitAnything;
}


bool HittableList::boundingBox(float time0, float time1, AABB &box) const
{
	if (size < 1) return false;

	AABB tmpBox;
	if (list[0]->boundingBox(time0, time1, tmpBox)) {
		box = tmpBox;
	}
	else {
		return false;
	}

	for (int i = 1; i < size; ++i)
	{
		if (list[i]->boundingBox(time0, time1, tmpBox))
		{
			box = surroundingBox(box, tmpBox);
		}
		else

			return false;
	}

	return true;
}


float HittableList::pdfVal(const vec3 &o, const vec3 &v) const {
	float weight = 1.0 / this->size;
	float sum = 0;
	for (int i = 0; i < this->size; i++)
	{
		sum += weight * list[i]->pdfVal(o, v);
	}

	return sum;
}

vec3 HittableList::random(const vec3 &o) const {
	int index = int(randCanonical() * this->size);

	while (index >= this->size)
	{
		index = int(randCanonical() * this->size);
	}

	return list[index]->random(o);
}
