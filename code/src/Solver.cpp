#include "Solver.h"

void Solver::UpdateParticles(Ball s, glm::vec3 forces, float dt)
{
	std::cout << "Default solver" << std::endl;
}

void Euler::UpdateSphere(Ball &s, glm::vec3 forces, float dt)
{
	s.pos += s.vel * dt;
	s.vel += (forces / s.mass) * dt;
}