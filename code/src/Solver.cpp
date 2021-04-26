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
		if ( i == 0 || i == 13) {}
		else
		{
			glm::vec3 tmp = ps.pos[i];
			ps.pos[i] += (ps.pos[i] - ps.prevPos[i]) + (forces[i] / ps.mass) * powf(dt, 2.0f);
			ps.prevPos[i] = tmp;

			ps.vel[i] += (ps.pos[i] - ps.prevPos[i]) / dt;
		}
	}
}