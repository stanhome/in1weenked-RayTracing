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