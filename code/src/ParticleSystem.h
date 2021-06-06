#pragma once

#include <glm/vec3.hpp>	

#include <glm/gtx/string_cast.hpp>
#include <iostream>

#pragma region Forward Declaration

namespace LilSpheres {
	extern const int maxParticles;
	extern int firstParticleIdx;
	extern int particleCount;
	extern void updateParticles(int startIdx, int count, float* array_data);
}

#pragma endregion

class ParticleSystem
{
protected:

	int maxParticles;	// TotalParticles
	
public:
	glm::vec3* pos;
	glm::vec3* initialPos;
	float mass;

	ParticleSystem(int numParticles = 100);
	int GetMaxParticles() { return maxParticles; }
};

