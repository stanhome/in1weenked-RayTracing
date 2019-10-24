// RayTracing.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
#include <fstream>


#include "utils.h"
#include "ray.h"
#include "hitable.h"
#include "HitableList.h"
#include "sphere.h"
#include "Camera.h"
#include "Material.h"

using namespace std;

const char *FILE_PATH = "output/ch9-Dielectrics.ppm";

const float MAX_RAY_HIT_DISTANCE = 1000.0;
// 光线追踪最大次数
const int RAY_TRACE_MAX_TIMES = 50;


vec3 color(const Ray &r, Hitable *world, int depth) {
	HitRecord rec;
	if (world->hit(r, 0.0, MAX_RAY_HIT_DISTANCE, rec)) {
		Ray scattered;
		vec3 attenuation;
		
		if (depth < RAY_TRACE_MAX_TIMES && rec.matPtr->scatter(r, rec, attenuation, scattered))
		{
			return attenuation * color(scattered, world, depth + 1);
		}
		else {
			return vec3(0, 0, 0);
		}
	}
	else {
		// sky background
		vec3 direction = r.direction();
		float t = 0.5 * direction.y + 0.5;

		return (1.0 - t) * vec3::ONE + t * vec3(0.5, 0.7, 1.0);
	}
}

int main()
{
	initUtils();

	float nx = 200;
	float ny = 100;
	float ns = 100;

	ofstream ppmImage(FILE_PATH);
	ppmImage << "P3\n" << nx << " " << ny << "\n255\n";
	ppmImage.close();

	// init world objects;
	Hitable *list[5];
	list[0] = new Sphere(vec3(0, 0, -1), 0.5, new Lambertian(vec3(0.1, 0.2, 0.5)));
	list[1] = new Sphere(vec3(0, -100.5, -1), 100, new Lambertian(vec3(0.8, 0.8, 0.0))); // floor
	list[2] = new Sphere(vec3(1, 0, -1), 0.5, new Metal(vec3(0.8, 0.6, 0.2), 0.0));
	list[3] = new Sphere(vec3(-1, 0, -1), 0.5, new Dielectric(1.5));
	list[4] = new Sphere(vec3(-1, 0, -1), -0.45, new Dielectric(1.5)); // bubble
	Hitable *world = new HitableList(list, 5);
	Camera camera;

	// draw
	for (int j = ny - 1; j >= 0; j--)
	{
		ppmImage.open(FILE_PATH, ios::app);
		for (int i = 0; i < nx; i++)
		{
			vec3 col(0, 0, 0);

			for (int s = 0; s < ns; s++)
			{
				float u = float(i + randCanonical()) / nx;
				float v = float(j + randCanonical()) / ny;
				Ray r = camera.getRay(u, v);
				//vec3 p = r.pointAtParameter(2.0);
				col += color(r, world, 0);
			}

			col /= ns;
			// use gamma 2. so square-root
			col = vec3(sqrt(col.r()), sqrt(col.g()), sqrt(col.b()));

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

