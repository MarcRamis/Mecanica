#pragma once

#include <glm\glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/vec3.hpp>
#include <glm/gtx/string_cast.hpp>

#include <iostream>

class Collisions
{
public:
	
	// Collisions detection
	static bool CollisionBox(glm::vec3 pos, glm::vec3 boxMin, glm::vec3 boxMax);
	static bool CollisionSphere(glm::vec3 pos, glm::vec3 spherePos, float sphereRadius);
	static bool CollisionCapsule(glm::vec3 pos, glm::vec3 spherePos, float sphereRadius);
	
	// Bouncing
	static void CollionBoxBounce(glm::vec3& pos, glm::vec3& vel, glm::vec3 boxMin, glm::vec3 boxMax, float coef_elast);
	static void CollionSphereBounce(glm::vec3& pos, glm::vec3& vel, glm::vec3 spherePos, float sphereRadius, float coef_elast);
	static void CollionCapsuleBounce(glm::vec3& pos, glm::vec3& vel, glm::vec3 spherePosA, glm::vec3 spherePosB, float sphereRadius, float coef_elast);
};