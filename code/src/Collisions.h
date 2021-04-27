#pragma once

#include <glm\glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/vec3.hpp>

class Collisions
{
public:
	
	// Collisions detection
	static bool CollisionBox(glm::vec3 pos, glm::vec3 boxMin, glm::vec3 boxMax);
	static bool CollisionSphere(glm::vec3 pos, glm::vec3 spherePos, float sphereRadius);

	// Bouncing
	static glm::vec3 CollionBoxBounce(glm::vec3 pos, glm::vec3 boxMin, glm::vec3 boxMax);
	static glm::vec3 CollionSphereBounce(glm::vec3 pos, glm::vec3 spherePos, float sphereRadius);
};