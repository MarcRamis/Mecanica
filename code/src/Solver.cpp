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

void Verlet::UpdateParticles(ParticleSystem ps, glm::vec3* forces, float dt, SphereCollider sphere)
{
	for (int i = 0; i < ps.maxParticles; i++)
	{
		if ( i == 0 || i == 13) {}
		else
		{
			if (!Collisions::CollisionBox(ps.pos[i], glm::vec3(-5, 0, -5), glm::vec3(5, 10, 5)) && enableBox) // BOX DETECT COL
			{ 
				ps.pos[i] = Collisions::CollionBoxBounce(ps.pos[i], glm::vec3(-5, 0, -5), glm::vec3(5, 10, 5), ps.coef_elasticity);
				ps.prevPos[i] = Collisions::CollionBoxBounce(ps.prevPos[i], glm::vec3(-5, 0, -5), glm::vec3(5, 10, 5), ps.coef_elasticity);
			}	
			else if(Collisions::CollisionSphere(ps.pos[i], sphere.pos, sphere.radius) && sphere.enabled) // SPHERE DETECT COL
			{
				ps.pos[i] = Collisions::CollionSphereBounce(ps.pos[i], sphere.pos, sphere.radius, ps.coef_elasticity);
				ps.prevPos[i] = Collisions::CollionSphereBounce(ps.prevPos[i], sphere.pos, sphere.radius, ps.coef_elasticity);
			}
			else // SOLVER CALC
			{
				glm::vec3 tmp = ps.pos[i];
				ps.pos[i] += (ps.pos[i] - ps.prevPos[i]) + (forces[i] / ps.mass) * powf(dt, 2.0f);
				ps.prevPos[i] = tmp;

				ps.vel[i] = (ps.pos[i] - ps.prevPos[i]) / dt;
			}
		}
	}
}