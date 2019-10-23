#pragma once

#include "ray.h"

class Camera {
public:
	vec3 origin;
	vec3 lowerLeftCorner;
	vec3 horizontal;
	vec3 vertical;

public:
	Camera() {
		origin = vec3(0, 0, 0);
		lowerLeftCorner = vec3(-2.0, -1.0, -1.0);
		horizontal = vec3(4.0, 0, 0);
		vertical = vec3(0, 2, 0);
	}

	Ray getRay(float u, float v) {
		return Ray(origin, lowerLeftCorner + u * horizontal + v * vertical - origin);
	}
};