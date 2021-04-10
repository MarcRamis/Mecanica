#include "Solver.h"

void Solver::UpdateParticles(ParticleSystem ps, glm::vec3* forces, float dt)
{
	std::cout << "This is default solver" << std::endl;
}

void Euler::UpdateParticles(ParticleSystem ps, glm::vec3* forces, float dt)
{
	for (int i = 0; i < ps.maxParticles; i++)
	{
		ps.pos[i] += ps.vel[i] * dt;
		ps.vel[i] += ps.acc[i] * dt;
	}
}

void Verlet::UpdateParticles(ParticleSystem ps, glm::vec3* forces, float dt)
{
	//std::cout << ps.maxParticles << std::endl;
	for (int i = 0; i < ps.maxParticles; i++)
	{
		std::cout << "Prev pos" << glm::to_string(ps.prevPos[i]) << std::endl;
		std::cout << "Pos" << glm::to_string(ps.pos[i]) << std::endl;

		ps.prevPos[i] = ps.pos[i];
		
		ps.pos[i] += (ps.pos[i] - ps.prevPos[i]) + forces[i] * glm::pow(dt, 2.0f);
	}
}