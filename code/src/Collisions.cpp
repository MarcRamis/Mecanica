#include "Collisions.h"

bool Collisions::CollisionBox(glm::vec3 pos, glm::vec3 boxMin, glm::vec3 boxMax) {
	
	return (pos.x >= boxMin.x && pos.x <= boxMax.x) &&
		(pos.y >= boxMin.y && pos.y <= boxMax.y) &&
		(pos.z >= boxMin.z && pos.z <= boxMax.z);
}

bool Collisions::CollisionSphere(glm::vec3 pos, glm::vec3 spherePos, float sphereRadius)
{
	float distance = sqrt(pow(pos.x - spherePos.x,2) + pow(pos.y - spherePos.y, 2) + pow(pos.z - spherePos.z, 2));
	
	return distance < sphereRadius;
}