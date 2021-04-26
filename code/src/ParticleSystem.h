#pragma once

#include <glm/vec3.hpp>	

#include <glm/gtx/string_cast.hpp>
#include <iostream>
#include <vector>

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
	glm::vec3* forces;
	float mass;

	int maxParticles;	// TotalParticles

	ParticleSystem(int numParticles = 100);

	int GetMaxParticles() { return maxParticles; }
};

