#pragma once

#include "ray.h"

class Material;

struct HitRecord {
	// ��ײ���ߵĲ���
	float t;
	// �����������ײ��
	vec3 p;
	// ��ײ�㴦�ģ���ķ��ߣ��ѹ�һ����
	vec3 normal;
	Material *matPtr;
};

class Hitable {
public:
	virtual bool hit(const Ray &r, float tMin, float tMax, HitRecord &rec) const = 0;
};