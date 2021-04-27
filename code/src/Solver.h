#pragma once

#include "ParticleSystem.h"
#include "Collisions.h"
#include "Sphere.h"
#include "Utils.h"

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

	bool enableBox = true;
	bool enableSphere = false;
	
	void UpdateParticles(ParticleSystem ps, glm::vec3* forces, float dt, SphereCollider sphere);
};