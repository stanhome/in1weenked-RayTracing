/*
 * @Date: 2019-10-21 11:10:03
 * @LastEditors: Stan
 * @LastEditTime: 2019-10-21 14:55:15
 */
 
#pragma once

#include <math.h>
#include <stdlib.h>
#include <iostream>

class vec3
{
public:
	float x, y, z;

	static vec3 ZERO;
	static vec3 ONE;
	static vec3 UP;
	
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

	inline vec3& operator+=(const vec3 &v2) {
		x += v2.x;
		y += v2.y;
		z += v2.z;

		return *this;
	}
	//inline vec3& operator-=(const vec3 &v2);
	//inline vec3& operator*=(const vec3 &v2);
	//inline vec3& operator/=(const vec3 &v2);
	//inline vec3& operator*=(const float t);

	inline vec3& operator/=(const float t) {
		x /= t;
		y /= t;
		z /= t;
		
		return *this;
	}

	inline float length() const { return sqrt(x*x + y*y + z*z); }
	inline float squaredLength() const {return x * x + y * y + z * z; }

	inline vec3 &normalize() {
		float len = length();
		x /= len;
		y /= len;
		z /= len;

		return *this;
	}

	inline vec3 normalized() const {
		float len = length();
		return vec3(x / len, y / len, z / len);
	}

	inline static float dot(const vec3 &lhs, const vec3 &rhs) {
		return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
	}

	inline static vec3 cross(const vec3 &lhs, const vec3 &rhs) {
		return vec3((lhs.y * rhs.z - lhs.z * rhs.y),
			(lhs.z * rhs.x - lhs.x * rhs.z),
			(lhs.x * rhs.y - lhs.y * rhs.x)
		);
	}
};

inline vec3 operator+(const vec3 &lhs, const vec3 &rhs) {
	return vec3(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z);
}

inline vec3 operator-(const vec3 &lhs, const vec3 &rhs) {
	return vec3(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z);
}

inline vec3 operator*(const vec3 &lhs, const float s) {
	return vec3(lhs.x * s, lhs.y * s, lhs.z * s);
}

inline vec3 operator*(const float s, const vec3 &lhs) {
	return lhs * s;
}

inline vec3 operator/(const vec3 &lhs, const float s) {
	return vec3(lhs.x / s, lhs.y / s, lhs.z / s);
}


// for color
inline vec3 operator*(const vec3 &lhs, const vec3 &rhs) {
	return vec3(lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z);
}

// for color
inline vec3 operator/(const vec3 &lhs, const vec3 &rhs) {
	return vec3(lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z);
}


vec3 vec3::ZERO = vec3(0, 0, 0);
vec3 vec3::ONE = vec3(1, 1, 1);
vec3 vec3::UP = vec3(0, 1, 0);