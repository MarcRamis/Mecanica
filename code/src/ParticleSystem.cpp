#include "ParticleSystem.h"

/////////Forward declarations
namespace LilSpheres {
	extern const int maxParticles;
	extern int firstParticleIdx;
	extern int particleCount;
	extern void updateParticles(int startIdx, int count, float* array_data);
}

ParticleSystem::ParticleSystem(int _numParticles) : maxParticles(_numParticles),  currentParticles(0){
	pos = new glm::vec3[_numParticles];
	vel = new glm::vec3[_numParticles];
	acc = new glm::vec3[_numParticles];

	age = new float[_numParticles];
	
	mass = 1;

	gravity = glm::vec3(0.f, -9.81f, 0.f);

	// Emitter fountain variables inicialization
	f_PosOrigen = glm::vec3(0.f, 5.f, 0.f);
	f_Dir = glm::vec3(0.1f, 1.f, 0.1f);
	f_Angle = 40.f;

	// Emitter cascade variables inicialization
	pointA = glm::vec3(-3.0f, 5.0f, 0.0f);
	pointB = glm::vec3(0.0f, 5.0f, 0.0f);

	c_Dir = glm::vec3(0.0f, 0.0f, -1.0f);
	randomSpawn = glm::vec3(0.0f, 0.0f, 0.0f);

	isForcesActivated = false;
}

void ParticleSystem::UpdateParticle(int idx, glm::vec3 newPosition, glm::vec3 newVelocity, float dt)
{
	if (isForcesActivated)
	{
		acc[idx] = gravity;
	}
	else
	{
		acc[idx] = glm::vec3(0.f,0.f,0.f);
	}

	vel[idx] = newVelocity;
	pos[idx] = newPosition;	
}

void ParticleSystem::Update(float dt, glm::vec3 spherePos, float sphereRadius, bool sphereCollision)
{
	for (int i = 0; i < currentParticles; i++)
	{
		// Euler
		pos[i] += vel[i] * dt;
		vel[i] += acc[i] * dt;

		// Check Collisions & Bouncing
		if (!Collisions::CollisionBox(pos[i], glm::vec3(-5, 0, -5), glm::vec3(5, 10, 5))) // BOX DETECT COL
		{
			Collisions::CollionBoxBounce(pos[i], vel[i], glm::vec3(-5, 0, -5), glm::vec3(5, 10, 5), elastCoef);
		}
		if (Collisions::CollisionSphere(pos[i], spherePos, sphereRadius) && sphereCollision)
		{
			Collisions::CollionSphereBounce(pos[i], vel[i], spherePos,sphereRadius, elastCoef);
		}
	}
}

void ParticleSystem::CheckAndBounceBoxCollisions(glm::vec3 pos, glm::vec3 vel, glm::vec3 boxMin, glm::vec3 boxMax)
{
	glm::vec3 n;
	float d;

	//if (pos.x >= boxMax.x)
	//{
	//	n = glm::normalize(glm::vec3(-1, 0, 0));
	//	d = boxMax.x;
	//
	//	pos = pos - (1 + coef_elast) * (glm::dot(n, pos) + d) * n;
	//}
	//if (pos.x <= boxMin.x)
	//{
	//	n = glm::normalize(glm::vec3(-1, 0, 0));
	//	d = boxMin.x;
	//
	//	pos = pos - (1 + coef_elast) * (glm::dot(n, pos) + d) * n;
	//}

	if (pos.y >= boxMax.y)
	{
		n = glm::normalize(glm::vec3(0, -1, 0));
		d = boxMax.y;

		pos = pos - (1 + elastCoef) * (glm::dot(n, pos) + d) * n;
		vel = vel - (1 + elastCoef) * (glm::dot(n, vel)) * n;
	}

	if (pos.y <= boxMin.y)
	{
		n = glm::normalize(glm::vec3(0, 1, 0));
		d = boxMin.y;

		pos = pos - (1 + elastCoef) * (glm::dot(n, pos) + d) * n;
		vel = vel - (1 + elastCoef) * (glm::dot(n, vel)) * n;
	}

	//if (pos.z >= boxMax.z)
	//{
	//	n = glm::normalize(glm::vec3(0, 0, -1));
	//	d = boxMax.z;
	//
	//	pos = pos - (1 + coef_elast) * (glm::dot(n, pos) + d) * n;
	//}
	//if (pos.z <= boxMin.z)
	//{
	//	n = glm::normalize(glm::vec3(0, 0, -1));
	//	d = boxMin.z;
	//
	//	pos = pos - (1 + coef_elast) * (glm::dot(n, pos) + d) * n;
	//}
}

void ParticleSystem::UpdateLilSpheres()
{
	LilSpheres::firstParticleIdx = 0;
	LilSpheres::particleCount = currentParticles;
	float* first_float = &(pos[0].x);
	
	LilSpheres::updateParticles(0, maxParticles, first_float);
}

void ParticleSystem::SpawnParticle(glm::vec3 _pos, glm::vec3 _vel, float dt)
{
	UpdateParticle(currentParticles, _pos, _vel, dt);
	age[currentParticles] = 0.f;
	currentParticles++;
}

void ParticleSystem::UpdateAge(float dt) {

	for (int i = 0; i < currentParticles; i++)
	{
		age[i] += dt;
	}
}

template <typename T>
void ShiftLeft(int count, T* arr, int positionsToShift) {
	for (int i = 0; i < count - positionsToShift; i++)
	{
		arr[i] = arr[i + positionsToShift];
	}
}

void ParticleSystem::DestroyOldParticles(float maxAge) {

	int positionsToShift = 0;
	int currentAge = age[0];
	while (currentAge > maxAge && positionsToShift <= currentParticles) {
		positionsToShift++;
		currentAge = age[positionsToShift];
	}
	
	ShiftLeft(currentParticles, pos, positionsToShift);
	ShiftLeft(currentParticles, vel, positionsToShift);
	ShiftLeft(currentParticles, acc, positionsToShift);
	ShiftLeft(currentParticles, age, positionsToShift);
	
	currentParticles -= positionsToShift;
}