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

glm::vec3 Collisions::CollionBoxBounce(glm::vec3 pos, glm::vec3 boxMin, glm::vec3 boxMax)
{
	glm::vec3 n;
	float d;

	if (pos.x >= boxMax.x)
	{

	}
	if (pos.x <= boxMin.x)
	{

	}

	if (pos.y >= boxMax.y)
	{
		//n = glm::normalize(glm::cross(glm::vec3(-5, 10, -5), glm::vec3(5, 10, 5)));
		//d = (n.x * pos.x) + (n.y * pos.y) + (n.z * pos.z);

		//pos = pos - (1 + 1.0f) * (glm::dot(n, pos) + d) * n;
	}

	if (pos.y <= boxMin.y)
	{
		n = glm::normalize(glm::cross(glm::vec3(-5, 0, 5), glm::vec3(5, 0, 5)));
		d = (n.x * pos.x) + (n.y * pos.y) + (n.z * pos.z);
		
		pos = pos - (1 + 1.0f) * (glm::dot(n, pos) + d) * n;
	}

	if (pos.z >= boxMax.z)
	{
	}
	if (pos.z <= boxMin.z)
	{
	}
	


	return pos;
}

glm::vec3 Collisions::CollionSphereBounce(glm::vec3 pos, glm::vec3 spherePos, float sphereRadius)
{
	return pos;
}