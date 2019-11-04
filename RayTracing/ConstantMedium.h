#pragma once

#include "Hitable.h"
#include "Texture.h"

class ConstantMedium : public Hitable {
public:
	Hitable *boundary;
	float density;
	Material *mat;

public:
	ConstantMedium(Hitable *b, float d, Texture *a) : boundary(b), density(d) {
		mat = new Isotropic(a);
	}

	virtual bool hit(const Ray &r, float tMin, float tMax, HitRecord &rec) const override;
	virtual bool boundingBox(float time0, float time1, AABB &box) const override {
		return boundary->boundingBox(time0, time1, box);
	}
};

bool ConstantMedium::hit(const Ray &r, float tMin, float tMax, HitRecord &rec) const {
	const bool enableDebug = false;
	bool debugging = enableDebug && randCanonical() < 0.00001;

	HitRecord rec1, rec2;

	do 
	{
		if (!boundary->hit(r, -FLT_MAX, FLT_MAX, rec1)) break;
		if (!boundary->hit(r, rec1.t + 0.0001, FLT_MAX, rec2)) break;

		if (debugging) std::cerr << "\nt0 t1 " << rec1.t << " " << rec2.t << std::endl;

		if (rec1.t < tMin) rec1.t = tMin;
		if (rec2.t > tMax) rec2.t = tMax;

		if (rec1.t >= rec2.t) break;

		if (rec1.t < 0) rec1.t = 0;

		float distanceInsideBoundary = (rec2.t - rec1.t) * r.direction().length();
		float hitDistance = -(1/density) * log(randCanonical());

		if (hitDistance > distanceInsideBoundary) break;

		rec.t = rec1.t + hitDistance / r.direction().length();
		rec.p = r.pointAtParameter(rec.t);

		if (debugging)
		{
			std::cerr << "hitDistance = " << hitDistance << std::endl
				<< "rec.t = " << rec.t << std::endl
				<< "rec.p = " << rec.p << std::endl;
		}

		rec.normal = vec3::UP; // arbitrary
		rec.matPtr = mat;

		return true;
	} while (false);

	return false;
}
