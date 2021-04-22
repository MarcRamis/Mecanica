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
		ps.vel[i] += (forces[i] / ps.mass) * dt;
	}
}

void Verlet::UpdateParticles(ParticleSystem ps, glm::vec3* forces, float dt)
{
	for (int i = 0; i < ps.maxParticles; i++)
	{
		//std::cout << "Iteration " << i << std::endl;

		glm::vec3 tmp = ps.pos[i];
		ps.pos[i] += (ps.pos[i] - ps.prevPos[i]) + (forces[i] / ps.mass) * powf(dt, 2.0f);

		//std::cout << "Prevpos before updated " << i << " : " << glm::to_string(ps.prevPos[i]) << std::endl;

		ps.prevPos[i] = tmp;

		ps.vel[i] += (ps.pos[i] - ps.prevPos[i]) / dt;

		//std::cout << "Temppos " << i << " : " << glm::to_string(tmp) << std::endl;
		//std::cout << "Prevpos after updated " << i << " : " << glm::to_string(ps.prevPos[i]) << std::endl;
		//std::cout << "Pos " << i << " : " << glm::to_string(ps.pos[i]) << std::endl;
		
		//std::cout << std::endl;
	}
}