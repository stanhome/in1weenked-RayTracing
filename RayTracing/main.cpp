// RayTracing.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <random>
#include <chrono>

#include "ray.h"
#include "hitable.h"
#include "HitableList.h"
#include "sphere.h"
#include "Camera.h"

using namespace std;

const float MAX_RAY_HIT_DISTANCE = 1000.0;
const char *FILE_PATH = "output/ch7-Diffuse Materials.ppm";

float randCanonical();

vec3 randomInUnitSphere() {
	vec3 p;
	do 
	{
		p = 2.0 * vec3(randCanonical(), randCanonical(), randCanonical()) - vec3(1, 1, 1);
	} while (vec3::dot(p, p) >= 1.0);

	return p;
}

vec3 color(const Ray &r, Hitable *world) {
	HitRecord rec;
	if (world->hit(r, 0.0, MAX_RAY_HIT_DISTANCE, rec)) {
		// rec.p + rec.normal => 射线碰撞点的单位切球（单位球体与碰撞点相切）的球心
		vec3 target = rec.p + rec.normal + randomInUnitSphere();

		return 0.5 * color(Ray(rec.p, target - rec.p), world);
	}
	else {
		// sky background
		vec3 direction = r.direction();
		float t = 0.5 * direction.y + 0.5;

		return (1.0 - t) * vec3::ONE + t * vec3(0.5, 0.7, 1.0);
	}
}


std::default_random_engine s_randGenerator;

void init() {
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	s_randGenerator = std::default_random_engine(seed);
}

float randCanonical() {
	return std::generate_canonical<float, std::numeric_limits<float>::digits>(s_randGenerator);
}


int main()
{
	init();

	float nx = 200;
	float ny = 100;
	float ns = 100;

	ofstream ppmImage(FILE_PATH);
	ppmImage << "P3\n" << nx << " " << ny << "\n255\n";
	ppmImage.close();

	// init world objects;
	Hitable *list[2];
	list[0] = new Sphere(vec3(0, 0, -1), 0.5);
	list[1] = new Sphere(vec3(0, -100.5, -1), 100);
	Hitable *world = new HitableList(list, 2);
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
				col += color(r, world);
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

