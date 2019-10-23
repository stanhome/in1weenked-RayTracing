#pragma once

#include "utils.h"
#include "ray.h"
#include "Hitable.h"

class Material {
public:
	/**
	 * ɢ��
	 * @param attenuation, ˥��
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
		// rec.p + rec.normal => ������ײ��ĵ�λ���򣨵�λ��������ײ�����У�������
		vec3 target = rec.p + rec.normal + randomInUnitSphere();
		scattered = Ray(rec.p, target - rec.p);
		attenuation = albedo;

		return true;
	}
};

class Metal : public Material {
public:
	//������
	vec3 albedo;
	// �ֲڶ�
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
