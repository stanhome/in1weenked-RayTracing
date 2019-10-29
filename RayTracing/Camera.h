#pragma once

#include "ray.h"

class Camera {
public:
	vec3 origin;
	vec3 lowerLeftCorner;
	vec3 horizontal;
	vec3 vertical;
	vec3 u, v, w;
	float lensRadius;
	float time0, time1; // for shutter open/close times

public:
	/*
	 * @param verticalFov is top to bottom in degrees
	 * @param aspect Height over width
	 * @param aperture, π‚»¶
	 * @param focusDistance, for defocus blur.
	*/
	Camera(vec3 lookFrom, vec3 lookAt, float verticalFov, float aspect, float aperture, float focusDistance, float t0, float t1) : time0(t0), time1(t1) {

		lensRadius = aperture / 2;

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
		lowerLeftCorner = origin + (-halfWidth*focusDistance*u) + (-halfHeight*focusDistance*v) + (-focusDistance * w);
		horizontal = 2 * halfWidth * focusDistance * u;
		vertical = 2 * halfHeight * focusDistance * v;
	}

	Ray getRay(float s, float t) {
		vec3 rd = lensRadius * randomInUnitDisk();
		vec3 offset = u * rd.x + v * rd.y;
		vec3 rayOrigin = origin + offset;

		float time = time0 + randCanonical() * (time1 - time0);

		return Ray(rayOrigin, lowerLeftCorner + s * horizontal + t * vertical - rayOrigin, time);
	}
};