// ch1-OutputAnImage.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
#include <fstream>

using namespace std;

// output an image with PPM format
int main()
{
	ofstream ppmImage("helloWorld.ppm");

	int nx = 200;
	int ny = 100;
	//std::cout << "P3\n" << nx << " " << ny << "\n255\n";
	ppmImage << "P3\n" << nx << " " << ny << "\n255\n";

	for (int j = ny-1; j >=0; j--)
	{
		for (int i = 0; i < nx; i++)
		{
			float r = float(i) / float(nx);
			float g = float(j) / float(ny);
			float b = 0.2;

			int ir = int(255.99 * r);
			int ig = int(255.99 * g);
			int ib = int(255.99 * b);

			ppmImage << ir << " " << ig << " " << ib << "\n";
		}
	}

	cout << "well done." << endl;

    return 0;
}

