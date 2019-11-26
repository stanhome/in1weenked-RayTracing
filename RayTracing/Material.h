#pragma once

#include "utils.h"
#include "ray.h"
#include "Hittable.h"
#include "Texture.h"
#include "ONB.h"
#include "PDF.h"


struct ScatterRecord {
	Ray specularRay;
	bool isSpecular;
	//albedo
	vec3 attenuation;
	PDF *pdfPtr = nullptr;

	~ScatterRecord() {
		if (pdfPtr != nullptr)
		{
			delete pdfPtr;
		}
	}
};

class Material {
public:
	/**
	 * 散射
	 * @param attenuation, 衰减
	*/
	virtual bool scatter(const Ray &rIn, const HitRecord &rec, ScatterRecord &srec) const = 0;
	virtual float scatteringPDF(const Ray &rIn, const HitRecord &hrec, const Ray &scattered) const {
		return 0;
	}
	virtual vec3 emitted(const Ray& rIn, const HitRecord &rec, float u, float v, const vec3& pos) const { return vec3::ZERO; }
};


class Lambertian : public Material {
public:
	Texture *albedo;

public:
	Lambertian(Texture *a) : albedo(a) {}

	virtual bool scatter(const Ray &rIn, const HitRecord &hrec, ScatterRecord &srec) const override
	{
		srec.isSpecular = false;
		srec.attenuation = albedo->val(hrec.u, hrec.v, hrec.p);
		srec.pdfPtr = new CosinePdf(hrec.normal);

		return true;
	}

	virtual float scatteringPDF(const Ray &rIn, const HitRecord &rec, const Ray &scattered) const override {
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

	virtual bool scatter(const Ray &rIn, const HitRecord &hrec, ScatterRecord &srec) const {
		vec3 reflected = reflect(rIn.direction(), hrec.normal);
		srec.specularRay = Ray(hrec.p, reflected + fuzz * randomInUnitSphere());
		srec.attenuation = albedo;
		srec.isSpecular = true;
		srec.pdfPtr = 0;

		return true;
	}
};

class Dielectric : public Material {
public:
	//折射率
	float refIdx;

public:
	Dielectric(float ri) : refIdx(ri){}

	virtual bool scatter(const Ray &rIn, const HitRecord &rec, ScatterRecord &srec) const {
		srec.isSpecular = true;
		srec.pdfPtr = 0;
		srec.attenuation = vec3::ONE; // the glass surface absorbs nothing.

		vec3 outwardNormal;
		vec3 reflected = reflect(rIn.direction(), rec.normal);
		float niOverNt;
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
			reflectProb = 1.0;
			//scattered = Ray(rec.p, reflected);
		}

		if (randCanonical() < reflectProb) {
			// 全反射
			srec.specularRay = Ray(rec.p, reflected);
		}
		else {
			srec.specularRay = Ray(rec.p, refracted);
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
	virtual bool scatter(const Ray &rIn, const HitRecord &hrec, ScatterRecord &srec) const override {
		srec.isSpecular = true;
		srec.pdfPtr = 0;
		srec.specularRay = Ray(hrec.p, randomInUnitSphere());
		srec.attenuation = albedo->val(hrec.u, hrec.v, hrec.p);

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
	virtual bool scatter(const Ray &rIn, const HitRecord &rec, ScatterRecord &srec) const override {
		return false;
	}

	virtual vec3 emitted(const Ray& rIn, const HitRecord &rec, float u, float v, const vec3& pos) const override{
		if (vec3::dot(rec.normal, rIn.direction()) < 0.0)
		{
			return emit->val(u, v, pos);
		}
		else
		{
			return vec3::ZERO;
		}
	}
};

