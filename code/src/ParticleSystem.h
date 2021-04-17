#pragma once

#include <glm/vec3.hpp>	

#include <glm/gtx/string_cast.hpp>
#include <iostream>

namespace LilSpheres {
	extern const int maxParticles;
	extern int firstParticleIdx;
	extern int particleCount;
	extern void updateParticles(int startIdx, int count, float* array_data);
}

class ParticleSystem
{
private:


public:
	glm::vec3* pos;
	glm::vec3* prevPos;
	glm::vec3* vel;
	float mass;
	
	int maxParticles;	// TotalParticles

	// -- Forces
	bool isForcesActivated;

	ParticleSystem(int numParticles = 100);

	int GetMaxParticles() { return maxParticles; }
};

