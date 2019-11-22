#pragma once
#include "vec3.h"

// Ortho-normal Bases
class ONB {
private:
	vec3 _axis[3];

public:
	ONB() {}

	inline vec3 operator[](int i) const { return _axis[i]; }
	// s
	vec3 u() const { return _axis[0]; }
	// tangent
	vec3 v() const { return _axis[1]; }
	// normal
	vec3 w() const { return _axis[2]; } 

	vec3 local(float a, float b, float c) const { return a * u() + b * v() + c * w(); }
	vec3 local(const vec3 &a) const { return a.x * u() + a.y * v() + a.z * w(); }
	
	void buildFromW(const vec3 &n);
};

void ONB::buildFromW(const vec3 &n) {
	_axis[2] = n.normalized();

	vec3 a;
	if (fabs(_axis[2].x) > 0.9) {
		// Normal 趋于 x 方向, 避免 a 与 n 方向重合
		a = vec3(0, 1, 0);
	} else {
		a = vec3(1, 0, 0);
	}

	_axis[1] = vec3::cross(_axis[2], a).normalized();
	_axis[0] = vec3::cross(_axis[2], _axis[1]);
}
