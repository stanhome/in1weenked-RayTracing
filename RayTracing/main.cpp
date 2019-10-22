// RayTracing.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
#include <fstream>

#include "ray.h"
#include "hitable.h"
#include "HitableList.h"
#include "sphere.h"

using namespace std;

const float MAX_RAY_HIT_DISTANCE = 10000.0;


vec3 color(const Ray &r, Hitable *world) {
	HitRecord rec;
	if (world->hit(r, 0.0, MAX_RAY_HIT_DISTANCE, rec)) {
		return 0.5 * (rec.normal + vec3::ONE);
	}
	else {
		vec3 direction = r.direction();
		float t = 0.5 * direction.y + 0.5;

		return (1.0 - t) * vec3::ONE + t * vec3(0.5, 0.7, 1.0);
	}
}


const char *FILE_PATH = "output/ch5-surface normals and multiple objects.ppm";

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

	// init world objects;
	Hitable *list[2];
	list[0] = new Sphere(vec3(0, 0, -1), 0.5);
	list[1] = new Sphere(vec3(0, -100.5, -1), 100);
	Hitable *world = new HitableList(list, 2);

	// draw
	for (int j = ny - 1; j >= 0; j--)
	{
		ppmImage.open(FILE_PATH, ios::app);
		for (int i = 0; i < nx; i++)
		{
			float u = float(i) / float(nx);
			float v = float(j) / float(ny);
			Ray r(origin, lowerLeftCorner + u * horizontal + v * vertical);

			vec3 p = r.pointAtParameter(2.0);
			vec3 col = color(r, world);
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

