#pragma once

#include "utils.h"
#include "Hittable.h"

int boxXCompare(const void *a, const void *b) {
	AABB boxLeft, boxRight;
	Hittable *ah = *(Hittable**)a;
	Hittable *bh = *(Hittable**)b;

	if (!ah->boundingBox(0, 0, boxLeft) || !bh->boundingBox(0, 0, boxRight))
	{
		std::cerr << "no bounding box in BhvNode constructor" << std::endl;
	}

	if (boxLeft.minPos().x - boxRight.minPos().x < 0.0)
		return -1;
	else 
		return 1;
}

int boxYCompare(const void *a, const void *b) {
	AABB boxLeft, boxRight;
	Hittable *ah = *(Hittable**)a;
	Hittable *bh = *(Hittable**)b;

	if (!ah->boundingBox(0, 0, boxLeft) || !bh->boundingBox(0, 0, boxRight))
	{
		std::cerr << "no bounding box in BhvNode constructor" << std::endl;
	}

	if (boxLeft.minPos().y - boxRight.minPos().y < 0.0)
		return -1;
	else
		return 1;
}

int boxZCompare(const void *a, const void *b) {
	AABB boxLeft, boxRight;
	Hittable *ah = *(Hittable**)a;
	Hittable *bh = *(Hittable**)b;

	if (!ah->boundingBox(0, 0, boxLeft) || !bh->boundingBox(0, 0, boxRight))
	{
		std::cerr << "no bounding box in BhvNode constructor" << std::endl;
	}

	if (boxLeft.minPos().z - boxRight.minPos().z < 0.0)
		return -1;
	else
		return 1;
}

//Bounding Volume Hierarchies
class BvhNode : public Hittable {
public:
	Hittable *left;
	Hittable *right;
	AABB box;

public:
	BvhNode() {}
	BvhNode(Hittable **l, int n, float time0, float time1);
	virtual bool hit(const Ray &r, float tMin, float tMax, HitRecord &rec) const;
	virtual bool boundingBox(float time0, float time1, AABB &box) const;
};

BvhNode::BvhNode(Hittable **l, int n, float time0, float time1) {
	//int chooseAxis = int(3 * randCanonical());
	//if (chooseAxis == 0)
		qsort(l, n, sizeof(Hittable *), boxXCompare);
	//else if (chooseAxis == 1)
	//	qsort(l, n, sizeof(Hitable *), boxYCompare);
	//else
	//	qsort(l, n, sizeof(Hitable *), boxZCompare);

	do 
	{
		if (n == 1) {
			left = right = l[0];

			if (!left->boundingBox(time0, time1, box))
			{
				std::cerr << "no bounding box in BvhNode constructor." << std::endl;
			}
			break;
		}
		else if (n == 2) {
			left = l[0];
			right = l[1];
		}
		else {
			int leftCount = n / 2;

			left = new BvhNode(l, leftCount, time0, time1);
			right = new BvhNode(l + leftCount, n - leftCount, time0, time1);
		}

		AABB boxL, boxR;
		if (!left->boundingBox(time0, time1, boxL) || !right->boundingBox(time0, time1, boxR))
		{
			std::cerr << "no bounding box in BvhNode constructor." << std::endl;
		}

		box = surroundingBox(boxL, boxR);

	} while (0);

	
}

bool BvhNode::hit(const Ray &r, float tMin, float tMax, HitRecord &rec) const {

	do 
	{
		if (box.hit(r, tMin, tMax))
		{
			HitRecord leftRec, rightRec;
			bool isHitLeft = left->hit(r, tMin, tMax, leftRec);
			bool isHitRight = right->hit(r, tMin, tMax, rightRec);

			if (isHitLeft && isHitRight) {
				rec = leftRec.t < rightRec.t ? leftRec : rightRec;
				break;
			}
			else if (isHitLeft) {
				rec = leftRec;
				break;
			} else if (isHitRight) {
				rec = rightRec;
				break;
			}
		}

		return false;
	} while (false);

	return true;
}

bool BvhNode::boundingBox(float time0, float time1, AABB &b) const {
	b = this->box;

	return true;
}