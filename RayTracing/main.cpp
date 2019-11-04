#include "stdafx.h"
#include <iostream>
#include <sstream>
#include <thread>
#include <windows.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

#include "utils.h"
#include "ray.h"
#include "hitable.h"
#include "HitableList.h"
#include "sphere.h"
#include "Camera.h"
#include "Material.h"
#include "MovingSphere.h"
#include "Bvh.h"
#include "PlanRect.h"
#include "Box.h"

using namespace std;

const char *FILE_PATH = "output/next week/ch07-Instances.png";

const float MAX_RAY_HIT_DISTANCE = 10000.0;
// 光线追踪最大次数
const int RAY_TRACE_MAX_TIMES = 50;


vec3 color(const Ray &r, Hitable *world, int depth) {
	HitRecord rec;
	if (world->hit(r, 0.001, MAX_RAY_HIT_DISTANCE, rec)) {
		Ray scattered;
		vec3 attenuation;
		vec3 emitted = rec.matPtr->emitted(rec.u, rec.v, rec.p);

		if (depth < RAY_TRACE_MAX_TIMES && rec.matPtr->scatter(r, rec, attenuation, scattered))
		{
			return attenuation * color(scattered, world, depth + 1);
		}
		else {
			return emitted;
		}
	}
	else {
		// sky background
		//vec3 direction = r.direction();
		//float t = 0.5 * direction.y + 0.5;

		//return (1.0 - t) * vec3::ONE + t * vec3(0.5, 0.7, 1.0);

		return vec3::ZERO;
	}
}

Hitable *generateWorld() {
	//Hitable *list[5];
	//int i = 0;
	//list[i++] = new Sphere(vec3(0, 0, -1), 0.5, new Lambertian(new ConstantTexture(vec3(0.1, 0.2, 0.5))));
	//list[i++] = new Sphere(vec3(1, 0, -1), 0.5, new Metal(vec3(0.8, 0.6, 0.2), 0.0));
	//list[i++] = new Sphere(vec3(-1, 0, -1), 0.5, new Dielectric(1.5));
	////list[i++] = new Sphere(vec3(-1, 0, -1), -0.45, new Dielectric(1.5)); // bubble

	//Hitable **retList = new Hitable *[2];
	//retList[0] = new Sphere(vec3(0, -100.5, -1), 100, new Lambertian(new ConstantTexture(vec3(0.8, 0.8, 0.0)))); // floor

	////retList[1] = new HitableList(list, i);
	//retList[1] = new BvhNode(list, i, 0, 1);


	// Perlin Noise
	Texture *perlinTexture = new NoiseTexture(4);
	Hitable **retList = new Hitable *[2];
	retList[0] = new Sphere(vec3(0, -1000, -1), 1000, new Lambertian(perlinTexture));
	retList[1] = new Sphere(vec3(0, 2, 0), 2, new Lambertian(perlinTexture));

	return new HitableList(retList, 2);
}

Hitable *earth() {
	int width, height, chanel;
	unsigned char *textureData = stbi_load("res/earthmap.jpg", &width, &height, &chanel, 0);
	Material *mat = new Lambertian(new ImageTexture(textureData, width, height));

	return new Sphere(vec3(0, 0, 0), 2, mat);
}

Hitable *sampleLight() {
	Texture *perText = new NoiseTexture(4);
	Hitable **list = new Hitable*[4];
	int i = 0;
	list[i++] = new Sphere(vec3(0, -1000, 0), 1000, new Lambertian(perText));
	list[i++] = new Sphere(vec3(0, 2, 0), 2, new Lambertian(perText));
	list[i++] = new Sphere(vec3(0, 7, 0), 2, new DiffuseLight(new ConstantTexture(vec3(4, 4, 4))));
	list[i++] = new XYRect(3, 5, 1, 3, -2, new DiffuseLight(new ConstantTexture(vec3(4, 4, 4))));

	return new HitableList(list, i);
}

Hitable *cornellBox() {
	Hitable **list = new Hitable*[8];
	int i = 0;
	Material *red = new Lambertian(new ConstantTexture(vec3(0.65, 0.05, 0.05)));
	Material *white = new Lambertian(new ConstantTexture(vec3(0.73, 0.73, 0.73)));
	Material *green = new Lambertian(new ConstantTexture(vec3(0.12, 0.45, 0.15)));
	Material *light = new DiffuseLight(new ConstantTexture(vec3(15, 15, 15)));

	// look from -Z to Z
	// left 
	list[i++] = new FlipNormals(new YZRect(0, 555, 0, 555, 555, green));
	//right
	list[i++] = new YZRect(0, 555, 0, 555, 0, red);

	// light
	auto lightPlane = new XZRect(213, 343, 227, 332, 554, light);
	lightPlane->name = "Top light";
	list[i++] = lightPlane;

	// top
	list[i++] = new FlipNormals(new XZRect(0, 555, 0, 555, 555, white));
	// bottom
	list[i++] = new XZRect(0, 555, 0, 555, 0, white);
	// background
	list[i++] = new FlipNormals(new XYRect(0, 555, 0, 555, 555, white));


	// two blocks
	list[i++] = new Translate(
		new RotateY(new Box(vec3(0, 0, 0), vec3(165, 165, 165), white), -18),
		vec3(130, 0, 65));
	list[i++] = new Translate(
		new RotateY(new Box(vec3(0, 0, 0), vec3(165, 330, 165), white), 15),
		vec3(264, 0, 290));

	return new HitableList(list, i);
}

Hitable *randomScene() {
	Texture *checker = new CheckerTexture(new ConstantTexture(vec3(0.2, 0.3, 0.1)), new ConstantTexture(vec3(0.9, 0.9, 0.9)));

	int n = 500;
	Hitable **list = new Hitable *[n];
	Hitable *floor = new Sphere(vec3(0, -1000, -1), 1000, new Lambertian(checker)); // floor

	int i = 0;
	for (int a = -10; a < 10; a++)
	{
		for (int b = -10; b < 10; b++)
		{
			float chooseMat = randCanonical();
			vec3 center(a + 0.9 * randCanonical(), 0.2, b + 0.9 * randCanonical());
			if ((center - vec3(4, 0.2, 0)).length() > 0.9) {
				if (chooseMat < 0.7) {
					//diffuse
					list[i++] = new MovingSphere(center, center + vec3(0, 0.5* randCanonical(), 0), 0.0, 1.0,
						0.2, new Lambertian(new ConstantTexture(
							vec3(randCanonical() * randCanonical(),
								randCanonical() * randCanonical(),
								randCanonical() * randCanonical()))));
				}
				else if (chooseMat < 0.95) {
					//metal
					list[i++] = new Sphere(center, 0.2, new Metal(
						vec3(0.5 * (1 + randCanonical()), 
							0.5 * (1 + randCanonical()), 
							0.5 * (1 + randCanonical()))));
				}
				else {
					// glass
					list[i++] = new Sphere(center, 0.2, new Dielectric(1.5));
				}
			}
		}
	}

	list[i++] = new Sphere(vec3(0, 1, 0), 1.0, new Dielectric(1.5));
	list[i++] = new Sphere(vec3(-4, 1, 0), 1.0, new Lambertian(new ConstantTexture(vec3(0.4, 0.2, 0.1))));
	list[i++] = new Sphere(vec3(4, 1, 0), 1.0, new Metal(vec3(0.7, 0.6, 0.5)));


	Hitable **retList = new Hitable *[2];
	retList[0] = floor;
	//retList[1] = new HitableList(list, i);
	retList[1] = new BvhNode(list, i, 0.0, 1.0);

	return new HitableList(retList, 2);
}

#define MULTIPLE_RUN

/*
int nx = 800;
int ny = 400;
int ns = 20;

no Bounding Volume Hierarchies speed up ==> well done. elapsed:374.214s
with Bounding Volume Hierarchies speed up ==> well done. elapsed:156.208s
*/

int main()
{
	initUtils();

	int nx = 300;
	int ny = 300;
	int ns = 500;
	int n = 4;

	// init world objects;
	time_t now = time(0);
	printf("[%s]random scene begin...\n", ctime(&now));

	//Hitable *world = generateWorld();
	//Hitable *world = randomScene();
	//Hitable *world = earth();
	//Hitable *world = sampleLight();
	Hitable *world = cornellBox();

	now = time(0);
	printf("[%s]random scene end.\n", ctime(&now));

	//vec3 lookfrom(13, 2, 3);
	//vec3 lookat(0, 2, 0);

	vec3 lookfrom(278, 278, -800);
	vec3 lookat(278, 278, 0);
	//float distToFocus = (lookfrom - lookat).length();
	float distToFocus = 10.0;
	float vfov = 40.0;
	float aperture = 0.0;
	Camera camera(lookfrom, lookat, vfov, float(nx) / float(ny), aperture, distToFocus, 0.0, 1.0);

	unsigned char *data = new unsigned char[nx * ny * n];
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

		printf("thread finished scope(%d, %d)\n", yStart, yEnd);
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

