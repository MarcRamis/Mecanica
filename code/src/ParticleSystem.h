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
	glm::vec3* acc;
	float mass;
	
	int maxParticles;	// TotalParticles
	int currentParticles;	// VariableTotalParticles
	float* age;

	// Elasticity & Friccion
	float elastCoef = 0.f;
	float friccCoef = 1.f;

	// -- Emitter variables
	float emisisonRate = 50.f;
	float maxAge = 1.f;

	int type = 0;

	// Emitter Fountain Variables
	glm::vec3 f_PosOrigen;
	glm::vec3 f_Dir;
	float f_Angle;

	// Emitter Cascade Variables
	glm::vec3 pointA;
	glm::vec3 pointB;
	glm::vec3 c_Dir;
	glm::vec3 randomSpawn;
	float randomNum;

	// -- Forces
	bool isForcesActivated;
	glm::vec3 gravity;

	ParticleSystem(int numParticles = 100);
	void UpdateParticle(int idx, glm::vec3 newPosition, glm::vec3 newVelocity, float dt);
	void UpdateLilSpheres();
	void UpdateAge(float dt);
	void UpdateEuler(float dt, glm::vec3 spherePos, float sphereRadius, bool sphereCollision);
	void DestroyOldParticles(float maxAge);

	void SpawnParticle(glm::vec3 pos, glm::vec3 _vel, float dt);

	int GetCurrentParticles() { return currentParticles; }
	int GetMaxParticles() { return maxParticles; }
	glm::vec3 GetPosition() { return *pos; }
};

