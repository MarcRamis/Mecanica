#pragma once

#include "ParticleSystem.h"

class Solver
{
public:
	void UpdateParticles(ParticleSystem ps, glm::vec3* forces, float dt);
};

class Euler : public Solver {
public:
	void UpdateParticles(ParticleSystem ps, glm::vec3* forces, float dt);
};

class Verlet : public Solver {
public:
	void UpdateParticles(ParticleSystem ps, glm::vec3* forces, float dt);
};