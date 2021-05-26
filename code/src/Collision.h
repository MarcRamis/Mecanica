#pragma once

#include "RigidBody.h"

class Collision
{
private:
	
	void CollisionTimeY(RigidBody* rb, glm::vec3 contactPoints, glm::vec3 boxVector, glm::vec3 normal, float boxSide, float dt);
	void CollisionTimeX(RigidBody* rb, glm::vec3 contactPoints, glm::vec3 boxVector, glm::vec3 normal, float boxSide, float dt);
	void CollisionTimeZ(RigidBody* rb, glm::vec3 contactPoints, glm::vec3 boxVector, glm::vec3 normal, float boxSide, float dt);
	
	void ImpulseCorrection(RigidBody* rb, glm::vec3 contactPoint, glm::vec3 normal, float dt);
	
public:

	static void CollisionBoxWall();
};