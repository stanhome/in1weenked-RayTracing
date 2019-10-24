// RayTracing.cpp : 定义控制台应用程序的入口点。
//

#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "stdafx.h"
#include <iostream>
#include <sstream>
#include <thread>
#include "stb/stb_image_write.h"
#include <windows.h>


#include "utils.h"
#include "ray.h"
#include "hitable.h"
#include "HitableList.h"
#include "sphere.h"
#include "Camera.h"
#include "Material.h"

using namespace std;

const char *FILE_PATH = "output/ch10-PositionableCamera.png";

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

#define MULTIPLE_RUN

int main()
{
	initUtils();

	int nx = 200;
	int ny = 100;
	int ns = 100;
	int n = 4;


	// init world objects;
	Hitable *list[5];
	list[0] = new Sphere(vec3(0, 0, -1), 0.5, new Lambertian(vec3(0.1, 0.2, 0.5)));
	list[1] = new Sphere(vec3(0, -100.5, -1), 100, new Lambertian(vec3(0.8, 0.8, 0.0))); // floor
	list[2] = new Sphere(vec3(1, 0, -1), 0.5, new Metal(vec3(0.8, 0.6, 0.2), 0.0));
	list[3] = new Sphere(vec3(-1, 0, -1), 0.5, new Dielectric(1.5));
	list[4] = new Sphere(vec3(-1, 0, -1), -0.45, new Dielectric(1.5)); // bubble
	Hitable *world = new HitableList(list, 5);

	//float R = cos(M_PI/4);
	//Hitable *list[2];
	//list[0] = new Sphere(vec3(-R, 0, -1), R, new Lambertian(vec3(0, 0, 1)));
	//list[1] = new Sphere(vec3( R, 0, -1), R, new Lambertian(vec3(1, 0, 0)));
	//Hitable *world = new HitableList(list, 2);
	Camera camera(vec3(-2, 2, 1), vec3(0, 0, -1), 90, float(nx) / float(ny));


	unsigned char *data = new unsigned char[nx * ny * ns];
	auto draw = [&](int yStart, int yEnd) {
		for (int j = yStart; j < yEnd; j++)
		{
			for (int i = 0; i < nx; i++)
			{
				vec3 col(0, 0, 0);

				for (int s = 0; s < ns; s++)
				{
					float u = float(i + randCanonical()) / float(nx);
					float v = float(j + randCanonical()) / float(ny);
					Ray r = camera.getRay(u, v);
					//vec3 p = r.pointAtParameter(2.0);
					col += color(r, world, 0);
				}

				col /= float(ns);

				// use gamma 2. so square-root
				col = vec3(sqrt(col.r()), sqrt(col.g()), sqrt(col.b()));

				int ir = int(255.99 * col.r());
				int ig = int(255.99 * col.g());
				int ib = int(255.99 * col.b());

				data[j *nx * n + i * n + 0] = ir;
				data[j *nx * n + i * n + 1] = ig;
				data[j *nx * n + i * n + 2] = ib;
				data[j *nx * n + i * n + 3] = 255;
			}
		}
	};

	clock_t tickStart = clock();

	#ifdef MULTIPLE_RUN
		int cpuCount = thread::hardware_concurrency();
		cout << "CPU count:" << cpuCount << endl;

		int perCpuTask = ny / cpuCount;
		if (ny % cpuCount > 0)
		{
			perCpuTask++;
		}

		vector<thread> drawThreads;
		int taskIndex = 0;
		do 
		{
			int endIndex = taskIndex + perCpuTask;
			if (endIndex > ny)
			{
				endIndex = ny;
			}

			thread drawThread(draw, taskIndex, endIndex);
			drawThreads.push_back(std::move(drawThread));
			taskIndex = endIndex;

		} while (taskIndex < ny);

		for (auto iter = drawThreads.begin(); iter != drawThreads.end(); iter++)
		{
			iter->join();
		}

	#else
		cout << "single thread:" <<  endl;
		// draw
		draw(0, ny);

	#endif
	clock_t tickEnd = clock();

	stringstream ss;
	ss << "==> well done. elapsed:" << double(tickEnd - tickStart) / CLOCKS_PER_SEC << "s" << endl;
	cout << ss.str();
	OutputDebugStringA(ss.str().c_str());

	stbi_flip_vertically_on_write(true);
	stbi_write_png(FILE_PATH, nx, ny, n, data, nx * n);
	delete[] data;
}

