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

glm::vec3 Collisions::CollionBoxBounce(glm::vec3 pos, glm::vec3 boxMin, glm::vec3 boxMax, float coef_elast)
{
	glm::vec3 n;
	float d;
	// SI NO EXPLOTA HACIA UN LADO EXPLOTA HACIA EL OTRO
	if (pos.x >= boxMax.x)
	{
		n = glm::normalize(glm::vec3(-1, 0, 0));
		d = boxMax.x;

		pos = pos - (1 + coef_elast) * (glm::dot(n, pos) + d) * n;
	}
	if (pos.x <= boxMin.x)
	{
		n = glm::normalize(glm::vec3(1, 0, 0));
		d = boxMin.x;

		pos = pos - (1 + coef_elast) * (glm::dot(n, pos) + d) * n;
	}

	if (pos.y >= boxMax.y)
	{
		n = glm::normalize(glm::vec3(0, -1, 0));
		d = boxMax.y;

		pos = pos - (1 + coef_elast) * (glm::dot(n, pos) + d) * n;

		//n = glm::normalize(glm::cross(glm::vec3(-5, 10, -5), glm::vec3(5, 10, 5)));
		//d = (n.x * pos.x) + (n.y * pos.y) + (n.z * pos.z);

		//pos = pos - (1 + 1.0f) * (glm::dot(n, pos) + d) * n;
		
		// Trying to implement friction
		//glm::vec3 vN = (glm::dot(glm::vec3(0,-1,0)), vel[i])) * glm::normalize(glm::cross(glm::vec3(-5, 0, 5), glm::vec3(5, 0, 5)));
		//glm::vec3 vT = vel[i] - vN;

		//vel[i] = vel[i] - friccCoef * vT;
	}
	
	if (pos.y <= boxMin.y)
	{
		n = glm::normalize(glm::vec3(0,1,0));
		d = boxMin.y;
		
		pos = pos - (1 + coef_elast) * (glm::dot(n, pos) + d) * n;
	}

	if (pos.z >= boxMax.z)
	{
		n = glm::normalize(glm::vec3(0, 0, 1));
		d = boxMax.z;

		pos = pos - (1 + coef_elast) * (glm::dot(n, pos) + d) * n;
	}
	if (pos.z <= boxMin.z)
	{
		n = glm::normalize(glm::vec3(0, 0, -1));
		d = boxMin.z;

		pos = pos - (1 + coef_elast) * (glm::dot(n, pos) + d) * n;
	}

	return pos;
}

glm::vec3 Collisions::CollionSphereBounce(glm::vec3 pos, glm::vec3 spherePos, float sphereRadius)
{
	return pos;
}