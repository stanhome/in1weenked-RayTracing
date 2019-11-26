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
#include "Hittable.h"
#include "HittableList.h"
#include "sphere.h"
#include "Camera.h"
#include "Material.h"
#include "MovingSphere.h"
#include "Bvh.h"
#include "PlanRect.h"
#include "Box.h"
#include "ConstantMedium.h"
#include "PDF.h"

using namespace std;

const char *FILE_PATH = "output/the rest of your life/ch12-Cleaning up PDF Management.png";

const float MAX_RAY_HIT_DISTANCE = 10000.0;
// 光线追踪最大次数
const int RAY_TRACE_MAX_TIMES = 50;


vec3 color(const Ray &r, Hittable *world, Hittable *lightShape, int depth) {
	HitRecord hrec;
	if (world->hit(r, 0.001, MAX_RAY_HIT_DISTANCE, hrec)) {
		ScatterRecord srec;
		vec3 emitted = hrec.matPtr->emitted(r, hrec, hrec.u, hrec.v, hrec.p);
		if (depth < RAY_TRACE_MAX_TIMES && hrec.matPtr->scatter(r, hrec, srec))
		{
			if (srec.isSpecular) {
				return srec.attenuation *color(srec.specularRay, world, lightShape, depth + 1);
			}
			else {
				HittablePdf plight(lightShape, hrec.p);
				MixturePdf p(&plight, srec.pdfPtr);
				Ray scattered = Ray(hrec.p, p.generate(), r.time());
				float pdfVal = p.val(scattered.direction());

				return emitted + srec.attenuation * hrec.matPtr->scatteringPDF(r, hrec, scattered) * color(scattered, world, lightShape, depth + 1) / pdfVal;
			}
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

Hittable *generateWorld() {
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
	Hittable **retList = new Hittable *[2];
	retList[0] = new Sphere(vec3(0, -1000, -1), 1000, new Lambertian(perlinTexture));
	retList[1] = new Sphere(vec3(0, 2, 0), 2, new Lambertian(perlinTexture));

	return new HittableList(retList, 2);
}

Hittable *earth() {
	int width, height, chanel;
	unsigned char *textureData = stbi_load("res/earthmap.jpg", &width, &height, &chanel, 0);
	Material *mat = new Lambertian(new ImageTexture(textureData, width, height));

	return new Sphere(vec3(0, 0, 0), 2, mat);
}

Hittable *sampleLight() {
	Texture *perText = new NoiseTexture(4);
	Hittable **list = new Hittable*[4];
	int i = 0;
	list[i++] = new Sphere(vec3(0, -1000, 0), 1000, new Lambertian(perText));
	list[i++] = new Sphere(vec3(0, 2, 0), 2, new Lambertian(perText));
	list[i++] = new Sphere(vec3(0, 7, 0), 2, new DiffuseLight(new ConstantTexture(vec3(4, 4, 4))));
	list[i++] = new XYRect(3, 5, 1, 3, -2, new DiffuseLight(new ConstantTexture(vec3(4, 4, 4))));

	return new HittableList(list, i);
}

Hittable *room() {
	Hittable **list = new Hittable*[5];
	Material *red = new Lambertian(new ConstantTexture(vec3(0.65, 0.05, 0.05)));
	Material *white = new Lambertian(new ConstantTexture(vec3(0.73, 0.73, 0.73)));
	Material *green = new Lambertian(new ConstantTexture(vec3(0.12, 0.45, 0.15)));

	int i = 0;
	// look from -Z to Z
	// left 
	list[i++] = new FlipNormals(new YZRect(0, 555, 0, 555, 555, green));
	//right
	list[i++] = new YZRect(0, 555, 0, 555, 0, red);
	// top
	list[i++] = new FlipNormals(new XZRect(0, 555, 0, 555, 555, white));
	// bottom
	list[i++] = new XZRect(0, 555, 0, 555, 0, white);
	// background
	list[i++] = new FlipNormals(new XYRect(0, 555, 0, 555, 555, white));

	return new HittableList(list, 5);
}

void cornellBox(Hittable **scene, Camera **camera, float aspect) {
	Hittable **list = new Hittable*[8];
	int i = 0;
	Material *light = new DiffuseLight(new ConstantTexture(vec3(15, 15, 15)));

	// light
	auto lightPlane = new XZRect(213, 343, 227, 332, 554, light);
	lightPlane->name = "Top light";
	list[i++] = new FlipNormals(lightPlane);

	list[i++] = room();

	Material *white = new Lambertian(new ConstantTexture(vec3(0.73, 0.73, 0.73)));
	//Material *aluminum = new Metal(vec3(0.8, 0.85, 0.88), 0.0); //铝
	Material *glass = new Dielectric(1.5);

	// two blocks
	//list[i++] = new Translate(
	//	new RotateY(new Box(vec3(0, 0, 0), vec3(165, 165, 165), white), -18),
	//	vec3(130, 0, 65));

	list[i++] = new Sphere(vec3(190, 90, 190), 90, glass);
	list[i++] = new Translate(
		new RotateY(new Box(vec3(0, 0, 0), vec3(165, 330, 165), white), 15),
		vec3(265, 0, 295));

	*scene = new HittableList(list, i);


	vec3 lookfrom(278, 278, -800);
	vec3 lookat(278, 278, 0);
	float distanceToFocus = 10.0;
	float aperture = 0.0;
	float vfov = 40.0;
	//vec3 lookfromRotate = focusRotate(lookfrom, lookat, 20);
	*camera = new Camera(lookfrom, lookat, vfov, aspect, aperture, distanceToFocus, 0.0, 1.0);
}

Hittable *cornellSmoke() {
	Hittable **list = new Hittable*[20];
	int i = 0;
	Material *light = new DiffuseLight(new ConstantTexture(vec3(7, 7, 7)));
	// light
	auto lightPlane = new XZRect(113, 443, 127, 432, 554, light);
	lightPlane->name = "Top light";
	list[i++] = lightPlane;

	list[i++] = room();

	Material *white = new Lambertian(new ConstantTexture(vec3(0.73, 0.73, 0.73)));

	// two blocks
	Hittable *b1 = new Translate(
		new RotateY(new Box(vec3(0, 0, 0), vec3(165, 165, 165), white), -18),
		vec3(130, 0, 65));
	Hittable *b2 = new Translate(
		new RotateY(new Box(vec3(0, 0, 0), vec3(165, 330, 165), white), 15),
		vec3(264, 0, 290));

	list[i++] = new ConstantMedium(b1, 0.01, new ConstantTexture(vec3::ONE));
	list[i++] = new ConstantMedium(b2, 0.01, new ConstantTexture(vec3::ZERO));

	return new HittableList(list, i);
}

Hittable *finalNextWeek() {
	int nb = 20;
	Hittable **list = new Hittable*[30];
	Hittable **boxList = new Hittable*[10000];
	Hittable **boxList2 = new Hittable*[10000];

	Material *white = new Lambertian(new ConstantTexture(vec3(0.73, 0.73, 0.73)));
	Material *ground = new Lambertian(new ConstantTexture(vec3(0.48, 0.83, 0.53)));
	// generate ground
	int b = 0;
	for (int i = 0; i < nb; i++)
	{
		for (int j = 0; j < nb; j++)
		{
			float w = 100;
			float x0 = -1000 + i * w;
			float z0 = -1000 + j * w;
			float y0 = 0;
			float x1 = x0 + w;
			float y1 = 100 * (randCanonical() + 0.01);
			float z1 = z0 + w;
			boxList[b++] = new Box(vec3(x0, y0, z0), vec3(x1, y1, z1), ground);
		}
	}

	int l = 0;
	list[l++] = new BvhNode(boxList, b, 0, 1);

	// light
	Material *light = new DiffuseLight(new ConstantTexture(vec3(7, 7, 7)));
	list[l++] = new XZRect(123, 423, 147, 412, 554, light);

	vec3 center(400, 400, 200);
	list[l++] = new MovingSphere(center, center + vec3(30, 0, 0), 0, 1, 50,
		new Lambertian(new ConstantTexture(vec3(0.7, 0.3, 0.1))));

	list[l++] = new Sphere(vec3(260, 150, 45), 50, new Dielectric(1.5));
	list[l++] = new Sphere(vec3(0, 150, 145), 50, new Metal(vec3(0.8, 0.8, 0.9), 10.0));

	Hittable *boundary = new Sphere(vec3(360, 150, 145), 70, new Dielectric(1.5));
	list[l++] = boundary;
	list[l++] = new ConstantMedium(boundary, 0.2, new ConstantTexture(vec3(0.2, 0.4, 0.9)));

	boundary = new Sphere(vec3(0, 0, 0), 5000, new Dielectric(1.5));
	list[l++] = new ConstantMedium(boundary, 0.0001, new ConstantTexture(vec3(1.0, 1.0, 1.0)));

	int width, height, chanel;
	unsigned char *textureData = stbi_load("res/tea.jpg", &width, &height, &chanel, 0);
	Material *emat = new Lambertian(new ImageTexture(textureData, width, height));
	list[l++] = new Translate(new RotateY(new Sphere(vec3::ZERO, 100, emat), 45), vec3(400, 200, 400));

	Texture *perText = new NoiseTexture(0.1);
	list[l++] = new Sphere(vec3(220, 280, 300), 80, new Lambertian(perText));

	int ns = 1000;
	for (int j = 0; j < ns; j++)
	{
		boxList2[j] = new Sphere(
			vec3(165 * randCanonical(), 165 * randCanonical(), 165 * randCanonical()),
			10, white);
	}
	list[l++] = new Translate(new RotateY(
		new BvhNode(boxList2, ns, 0.0, 1.0), 15), vec3(-100, 270, 395));

	return new HittableList(list, l);
}

Hittable *randomScene() {
	Texture *checker = new CheckerTexture(new ConstantTexture(vec3(0.2, 0.3, 0.1)), new ConstantTexture(vec3(0.9, 0.9, 0.9)));

	int n = 500;
	Hittable **list = new Hittable *[n];
	Hittable *floor = new Sphere(vec3(0, -1000, -1), 1000, new Lambertian(checker)); // floor

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


	Hittable **retList = new Hittable *[2];
	retList[0] = floor;
	//retList[1] = new HitableList(list, i);
	retList[1] = new BvhNode(list, i, 0.0, 1.0);

	return new HittableList(retList, 2);
}

vec3 focusRotate(const vec3 &lookFrom, const vec3 &lookAt, float rotateYDegree)
{
	vec3 look = lookFrom - lookAt;

	float cameraRotateYFocusObject = DEG_2_RAD * (rotateYDegree);
	float sinTheta = sin(cameraRotateYFocusObject);
	float cosTheat = cos(cameraRotateYFocusObject);

	float x = look.x * cosTheat - look.z * sinTheta;
	float z = look.x * sinTheta + look.z *cosTheat;

	look.x = x;
	look.z = z;

	look += lookAt;

	return look;
}

#define MULTIPLE_RUN

/*
int nx = 800;
int ny = 400;
int ns = 20;

no Bounding Volume Hierarchies speed up ==> well done. elapsed:374.214s
with Bounding Volume Hierarchies speed up ==> well done. elapsed:156.208s

with light PDF Sample ==> well done. elapsed:5.431s
*/

int main()
{
	initUtils();

	int nx = 500;
	int ny = 500;
	int ns = 1000;
	int n = 4;

	// init world objects;
	time_t now = time(0);
	printf("[%s]random scene begin...\n", ctime(&now));

	Hittable *world;
	Camera *camera;
	float aspect = float(ny) / float(nx);
	cornellBox(&world, &camera, aspect);
	Hittable *lightShape = new XZRect(213, 343, 227, 332, 554, 0);
	Hittable *glassSphere = new Sphere(vec3(190, 90, 190), 90, 0);
	Hittable *pdfList[2];
	pdfList[0] = lightShape;
	pdfList[1] = glassSphere;
	HittableList hlist(pdfList, 2);

	now = time(0);
	printf("[%s]random scene end.\n", ctime(&now));

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
					Ray r = camera->getRay(u, v);
					//vec3 p = r.pointAtParameter(2.0);
					col += color(r, world, &hlist, 0);
				}

				col /= float(ns);

				// use gamma 2. so square-root
				col = vec3(sqrt(col.r()), sqrt(col.g()), sqrt(col.b()));

				int ir = int(255.99 * col.r());
				int ig = int(255.99 * col.g());
				int ib = int(255.99 * col.b());

				data[j *nx * n + i * n + 0] = ir > 255 ? 255 : ir;
				data[j *nx * n + i * n + 1] = ig > 255 ? 255 : ig;
				data[j *nx * n + i * n + 2] = ib > 255 ? 255 : ib;
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

