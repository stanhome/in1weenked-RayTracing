// ch2-Vec3.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
#include <fstream>

#include "vec3.h"

using namespace std;

// output an image with PPM format
int main()
{
	ofstream ppmImage("helloWorld.ppm");

	int nx = 200;
	int ny = 100;
	//std::cout << "P3\n" << nx << " " << ny << "\n255\n";
	ppmImage << "P3\n" << nx << " " << ny << "\n255\n";

	for (int j = ny - 1; j >= 0; j--)
	{
		for (int i = 0; i < nx; i++)
		{
			vec3 col(float(i) / float(nx), float(j) / float(ny), 0.2);

			int ir = int(255.99 * col.r());
			int ig = int(255.99 * col.g());
			int ib = int(255.99 * col.b());

			ppmImage << ir << " " << ig << " " << ib << "\n";
		}
	}

	cout << "well done." << endl;

	return 0;
}