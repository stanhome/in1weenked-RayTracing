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
	Metal(const vec3 &a, float f) : albedo(a) {
		fuzz = f < 1 ? f : 1;
	}

	virtual bool scatter(const Ray &rIn, const HitRecord &rec, vec3 &attenuation, Ray &scattered) const {
		vec3 reflected = reflect(rIn.direction(), rec.normal);
		scattered = Ray(rec.p, reflected + fuzz * randomInUnitSphere());
		attenuation = albedo;

		return vec3::dot(scattered.direction(), rec.normal) > 0;
	}
};
