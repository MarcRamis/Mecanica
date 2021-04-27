#pragma once

#include <glm/vec3.hpp>

class Collisions
{
public:

	static bool CollisionBox(glm::vec3 pos, glm::vec3 boxMin, glm::vec3 boxMax);
	static bool CollisionSphere(glm::vec3 pos, glm::vec3 spherePos, float sphereRadius);
};