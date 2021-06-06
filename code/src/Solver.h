#pragma once

#include <iostream>

#include "Sphere.h"

class Solver
{
public:
	void UpdateParticles(Ball s, glm::vec3 forces, float dt);
};

class Euler : public Solver {
public:
	void UpdateSphere(Ball &s, glm::vec3 forces, float dt);
};