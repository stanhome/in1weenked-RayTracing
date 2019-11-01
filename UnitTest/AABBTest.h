#pragma once

#include "aabb.h"

const float MIN_RAY_CAST = -100.0f;
const float MAX_RAY_CAST = 100.0f;

static char *t_AABB()
{
	AABB ab(vec3::ZERO, vec3::ONE);

	Ray r1(vec3(-1, 0, 0.5), vec3(1, 0.5, 0));
	mu_assert("Ray 1 shoud interset with ab", ab.hit(r1, MIN_RAY_CAST, MAX_RAY_CAST) == true);

	Ray r2(vec3(-1, 0, 0.5), vec3(1, 2, 0));
	mu_assert("Ray 2 shoud not interset with ab", ab.hit(r2, MIN_RAY_CAST, MAX_RAY_CAST) == false);

	return 0;
}