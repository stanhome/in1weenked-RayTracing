#pragma once

#include "utils.h"
#include "ray.h"
#include "Hitable.h"


class Material {
public:
	/**
	 * 散射
	 * @param attenuation, 衰减
	*/
	virtual bool scatter(const Ray &rIn, const HitRecord &rec, vec3 &attenuation, Ray &scattered) const = 0;
};


class Lambertian : public Material {
public:
	vec3 albedo;

public:
	Lambertian(const vec3 &a) : albedo(a) {}

	virtual bool scatter(const Ray &rIn, const HitRecord &rec, vec3 &attenuation, Ray &scattered) const
	{
		// rec.p + rec.normal => 射线碰撞点的单位切球（单位球体与碰撞点相切）的球心
		vec3 target = rec.p + rec.normal + randomInUnitSphere();
		scattered = Ray(rec.p, target - rec.p);
		attenuation = albedo;

		return true;
	}
};

class Metal : public Material {
public:
	//反射率
	vec3 albedo;
	// 粗糙度
	float fuzz;

public:
	Metal(const vec3 &a, float f = 0) : albedo(a) {
		fuzz = f < 1 ? f : 1;
	}

	virtual bool scatter(const Ray &rIn, const HitRecord &rec, vec3 &attenuation, Ray &scattered) const {
		vec3 reflected = reflect(rIn.direction(), rec.normal);
		scattered = Ray(rec.p, reflected + fuzz * randomInUnitSphere());
		attenuation = albedo;

		return vec3::dot(scattered.direction(), rec.normal) > 0;
	}
};

class Dielectric : public Material {
public:
	//折射率
	float refIdx;

public:
	Dielectric(float ri) : refIdx(ri){}

	virtual bool scatter(const Ray &rIn, const HitRecord &rec, vec3 &attenuation, Ray &scattered) const {
		vec3 outwardNormal;
		vec3 reflected = reflect(rIn.direction(), rec.normal);
		float niOverNt;
		attenuation = vec3::ONE; // the glass surface absorbs nothing.
		vec3 refracted;
		float reflectProb;
		float cosine;

		if (vec3::dot(rIn.direction(), rec.normal) > 0) {
			//光线由水 向空气
			outwardNormal = -rec.normal;
			niOverNt = refIdx;
			//cosine = refIdx * vec3::dot(rIn.direction(), rec.normal) / rIn.direction().length();
			cosine = vec3::dot(rIn.direction(), rec.normal);
		}
		else {
			//光线由 空气向水
			outwardNormal = rec.normal;
			niOverNt = 1.0 / refIdx;
			cosine = -vec3::dot(rIn.direction(), rec.normal);
		}

		if (refract(rIn.direction(), outwardNormal, niOverNt, refracted))
		{
			reflectProb = schlick(cosine, refIdx);
			//scattered = Ray(rec.p, refracted);
		}
		else
		{
			// 全反射
			scattered = Ray(rec.p, reflected);
			reflectProb = 1.0;
		}

		if (randCanonical() < reflectProb) {
			scattered = Ray(rec.p, reflected);
		}
		else {
			scattered = Ray(rec.p, refracted);
		}

		return true;
	}
};


