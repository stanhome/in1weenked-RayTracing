/*
 * @Date: 2019-10-21 11:10:03
 * @LastEditors: Stan
 * @LastEditTime: 2019-10-21 12:32:49
 */
 
#pragma once

#include <math.h>
#include <stdlib.h>
#include <iostream>

class vec3
{
public:
	float x, y, z;
	
public:
	vec3() {};
	vec3(float xx, float yy, float zz) :x(xx), y(yy), z(zz) {}


	inline float r() const { return x; }
	inline float g() const { return y; }
	inline float b() const { return z; }

	inline const vec3& operator+() const { return *this; }
	inline vec3 operator-() const { return vec3(-x, -y, -z); }
	// inline float operator[](int i) const { return e[i]; }
	// inline float& operator[](int i) { return e[i]; }

	//inline vec3& operator+=(const vec3 &v2);
	//inline vec3& operator-=(const vec3 &v2);
	//inline vec3& operator*=(const vec3 &v2);
	//inline vec3& operator/=(const vec3 &v2);
	//inline vec3& operator*=(const float t);

	inline vec3& operator/=(const float t) {
		return vec3(x / t, y / t, z / t);
	}

	inline float length() const { return sqrt(x*x + y*y + z*z); }
	inline float squaredLength() const {return x * x + y * y + z * z; }

	inline void normalize() {
		int len = length();
		x /= len;
		y /= len;
		z /= len;
	}

	inline static float dot(const vec3 &v1, const vec3 &v2) {
		return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
	}

	inline static vec3 corss(const vec3 &v1, const vec3 &v2) {
		return vec3((v1.y * v2.z - v1.z * v2.y),
			(v1.z * v2.x - v1.x * v2.z),
			(v1.x * v2.y - v1.y * v2.x)
		);
	}
};

inline vec3 operator+(const vec3 &v1, const vec3 &v2) {
	return vec3(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
}

inline vec3 operator-(const vec3 &v1, const vec3 &v2) {
	return vec3(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
}

// for color
inline vec3 operator*(const vec3 &v1, const vec3 &v2) {
	return vec3(v1.x * v2.x, v1.y * v2.y, v1.z * v2.z);
}

// for color
inline vec3 operator/(const vec3 &v1, const vec3 &v2) {
	return vec3(v1.x / v2.x, v1.y / v2.y, v1.z / v2.z);
}


