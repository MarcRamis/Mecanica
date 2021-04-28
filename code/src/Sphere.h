#pragma once

#include <glm\vec3.hpp>

struct SphereCollider
{
	glm::vec3 pos;
	float radius;
	bool enabled;

	SphereCollider();
	SphereCollider(glm::vec3 _pos, float _r) : pos(_pos), radius(_r) {}
};