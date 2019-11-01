#pragma once

#include "hitable.h"

class HitableList : public Hitable {
public:
	Hitable **list;
	int size;

public:
	HitableList() {}
	HitableList(Hitable **l, int n) : list(l), size(n) {}
	virtual bool hit(const Ray &r, float tMin, float tMax, HitRecord &rec) const;
	virtual bool boundingBox(float time0, float time1, AABB &box) const;
};

bool HitableList::hit(const Ray &r, float tMin, float tMax, HitRecord &rec) const {
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


bool HitableList::boundingBox(float time0, float time1, AABB &box) const
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
