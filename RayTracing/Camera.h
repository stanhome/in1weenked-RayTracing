#pragma once

#include "ray.h"

class Camera {
public:
	vec3 origin;
	vec3 lowerLeftCorner;
	vec3 horizontal;
	vec3 vertical;

public:
	/*
	 * verticalFov is top to bottom in degrees
	*/
	Camera(vec3 lookFrom, vec3 lookAt, float verticalFov, float aspect) {
		vec3 u, v, w;
		float theta = verticalFov * M_PI / 180;
		float halfHeight = tan(theta / 2);
		float halfWidth = aspect * halfHeight;

		origin = lookFrom;
		w = lookFrom-lookAt;
		w.normalize();
		u = vec3::cross(vec3::UP, w);
		u.normalize();
		v = vec3::cross(w, u);

		//lowerLeftCorner = vec3(-halfWidth, -halfHeight, -1.0);
		lowerLeftCorner = origin + (-halfWidth*u) + (-halfHeight*v) + (-w);
		horizontal = 2 * halfWidth * u;
		vertical = 2 * halfHeight * v;
	}

	Ray getRay(float u, float v) {
		return Ray(origin, lowerLeftCorner + u * horizontal + v * vertical - origin);
	}
};