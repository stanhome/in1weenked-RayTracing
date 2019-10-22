// RayTracing.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
#include <fstream>

#include "vec3.h"
#include "ray.h"

using namespace std;

bool hitSphere(const vec3 &center, float radius, const ray &r) {
	vec3 oc = r.origin() - center;
	float a = vec3::dot(r.direction(), r.direction());
	float b = 2 * vec3::dot(r.direction(), oc);
	float c = vec3::dot(oc, oc) - radius * radius;
	float discriminant = b*b - 4*a*c;
	return discriminant > 0;
}

vec3 color(const ray &r) {
	if (hitSphere(vec3(0, 0, -1), 0.5, r))
	{
		return vec3(1, 0, 0);
	}

	vec3 direction = r.direction();
	float t = 0.5 * direction.y + 0.5;

	return (1.0 - t) * vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
}

const char *FILE_PATH = "output/ch4-adding a sphere.ppm";

int main()
{
	int nx = 200;
	int ny = 100;

	ofstream ppmImage(FILE_PATH);
	ppmImage << "P3\n" << nx << " " << ny << "\n255\n";
	ppmImage.close();

	vec3 lowerLeftCorner(-2.0, -1.0, -1.0);
	vec3 horizontal(4.0, 0, 0);
	vec3 vertical(0, 2, 0);
	vec3 origin(0, 0, 0);

	for (int j = ny - 1; j >= 0; j--)
	{
		ppmImage.open(FILE_PATH, ios::app);
		for (int i = 0; i < nx; i++)
		{
			float u = float(i) / float(nx);
			float v = float(j) / float(ny);

			ray r(origin, lowerLeftCorner + u * horizontal + v * vertical);
			vec3 col = color(r);
			int ir = int(255.99 * col.r());
			int ig = int(255.99 * col.g());
			int ib = int(255.99 * col.b());

			ppmImage << ir << " " << ig << " " << ib << "\n";
		}
		ppmImage.close();
	}

	cout << "well done." << endl;
	//char a;
	//cin >> a;
}

