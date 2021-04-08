#include "Solver.h"

void Solver::UpdateParticles(ParticleSystem ps, glm::vec3* forces, float dt)
{

}

void Euler::UpdateParticles(ParticleSystem ps, glm::vec3* forces, float dt)
{
	for (int i = 0; i < ps.currentParticles; i++)
	{
		ps.pos[i] += ps.vel[i] * dt;
		ps.vel[i] += ps.acc[i] * dt;
	}
}

void Verlet::UpdateParticles(ParticleSystem ps, glm::vec3* forces, float dt)
{

}