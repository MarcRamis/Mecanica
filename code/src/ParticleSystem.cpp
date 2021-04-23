#include "ParticleSystem.h"

/////////Forward declarations
namespace LilSpheres {
	extern const int maxParticles;
	extern int firstParticleIdx;
	extern int particleCount;
	extern void updateParticles(int startIdx, int count, float* array_data);
}

ParticleSystem::ParticleSystem(int _numParticles) : maxParticles(_numParticles) {
	pos = new glm::vec3[_numParticles];
	prevPos = new glm::vec3[_numParticles];
	vel = new glm::vec3[_numParticles];	
	forces = new glm::vec3[_numParticles];	
	
	mass = 1.0f;
}