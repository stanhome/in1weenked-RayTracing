#pragma once

#include "utils.h"
#include "ray.h"
#include "Hittable.h"
#include "Texture.h"


class Material {
public:
	/**
	 * 散射
	 * @param attenuation, 衰减
	*/
	virtual bool scatter(const Ray &rIn, const HitRecord &rec, vec3 &attenuation, Ray &scattered, float &pdf) const = 0;
	virtual float scatteringPDF(const Ray &rIn, const HitRecord &rec, Ray &scattered) const {
		return 0;
	}
	virtual vec3 emitted(float u, float v, const vec3 &pos) const { return vec3::ZERO; }
};


class Lambertian : public Material {
public:
	Texture *albedo;

public:
	Lambertian(Texture *a) : albedo(a) {}

	virtual bool scatter(const Ray &rIn, const HitRecord &rec, vec3 &attenuation, Ray &scattered, float &pdf) const override
	{
		// rec.p + rec.normal => 射线碰撞点的单位切球（单位球体与碰撞点相切）的球心
		//vec3 target = rec.p + rec.normal + randomInUnitSphere();

		vec3 direction;
		do 
		{
			direction = randomInUnitSphere();
		} while (vec3::dot(direction, rec.normal) < 0);

		scattered = Ray(rec.p, direction.normalized(), rIn.time());
		attenuation = albedo->val(rec.u, rec.v, rec.p);
		pdf = 0.5 / M_PI;

		return true;
	}

	virtual float scatteringPDF(const Ray &rIn, const HitRecord &rec, Ray &scattered) const override {
		float cosine = vec3::dot(rec.normal, scattered.direction());
		if (cosine < 0)
		{
			return 0;
		}

		return cosine / M_PI;
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

	virtual bool scatter(const Ray &rIn, const HitRecord &rec, vec3 &attenuation, Ray &scattered, float &pdf) const {
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

	virtual bool scatter(const Ray &rIn, const HitRecord &rec, vec3 &attenuation, Ray &scattered, float &pdf) const {
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

// 各向同性
class Isotropic : public Material {
public:
	Texture *albedo;

public:
	Isotropic(Texture *a) : albedo(a) {}
	virtual bool scatter(const Ray &rIn, const HitRecord &rec, vec3 &attenuation, Ray &scattered, float &pdf) const override {
	
		scattered = Ray(rec.p, randomInUnitSphere());
		attenuation = albedo->val(rec.u, rec.v, rec.p);

		return true;
	}
};

//////////////////////////////////////////////////////////////////////////
/// self-emit material

class DiffuseLight : public Material {
public:
	Texture *emit;

public:
	DiffuseLight(Texture *e) : emit(e) {}
	virtual bool scatter(const Ray &rIn, const HitRecord &rec, vec3 &attenuation, Ray &scattered, float &pdf) const override {
		return false;
	}

	virtual vec3 emitted(float u, float v, const vec3 &pos) const override{
		return emit->val(u, v, pos);
	}
};

