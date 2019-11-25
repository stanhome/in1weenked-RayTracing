#pragma once

#include "vec3.h"

class Ray
{
private:
	vec3 _origin;
	vec3 _direction;
	float _time;

public:
	Ray() {}
	Ray(const vec3 &origin, const vec3 &direction, float time=0.0): _origin(origin), _direction(direction), _time(time) {
		_direction.normalize();
	}
	const vec3 &origin() const { return _origin; }
	const vec3 &direction() const { return _direction; }
	float time() const { return _time; }
	vec3 pointAtParameter(float t) const { return _origin + t * _direction; }
};