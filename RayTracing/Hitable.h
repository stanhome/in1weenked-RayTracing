#pragma once

#include "ray.h"
#include "aabb.h"

class Material;

struct HitRecord {
	// ��ײ���ߵĲ���
	float t;
	float u , v;
	// �����������ײ��
	vec3 p;
	// ��ײ�㴦�ģ���ķ��ߣ��ѹ�һ����
	vec3 normal;
	Material *matPtr;
};

class Hitable {
public:
	virtual bool hit(const Ray &r, float tMin, float tMax, HitRecord &rec) const = 0;
	virtual bool boundingBox(float time0, float time1, AABB &box) const = 0;
};