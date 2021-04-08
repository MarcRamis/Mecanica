#include "ParticleSystem.h"

/////////Forward declarations
namespace LilSpheres {
	extern const int maxParticles;
	extern int firstParticleIdx;
	extern int particleCount;
	extern void updateParticles(int startIdx, int count, float* array_data);
}

ParticleSystem::ParticleSystem(int _numParticles) : maxParticles(_numParticles), currentParticles(0) {
	pos = new glm::vec3[_numParticles];
	vel = new glm::vec3[_numParticles];
	
	
	acc = new glm::vec3[_numParticles];
	age = new float[_numParticles];

	//mass = 1;

	//gravity = glm::vec3(0.f, -9.81f * mass, 0.f);

	//// Emitter fountain variables inicialization
	//f_PosOrigen = glm::vec3(0.f, 5.f, 0.f);
	//f_Dir = glm::vec3(0.1f, 1.f, 0.1f);
	//f_Angle = 40.f;

	//// Emitter cascade variables inicialization
	//pointA = glm::vec3(-3.0f, 5.0f, 0.0f);
	//pointB = glm::vec3(0.0f, 5.0f, 0.0f);

	//c_Dir = glm::vec3(0.0f, 0.0f, -1.0f);
	//randomSpawn = glm::vec3(0.0f, 0.0f, 0.0f);

	//isForcesActivated = false;
}

void ParticleSystem::UpdateParticle(int idx, glm::vec3 newPosition, glm::vec3 newVelocity, float dt)
{
	if (isForcesActivated)
	{
		acc[idx] = gravity;
	}
	else
	{
		acc[idx] = glm::vec3(0.f, 0.f, 0.f);
	}

	vel[idx] = newVelocity;
	pos[idx] = newPosition;
}

void ParticleSystem::UpdateEuler(float dt, glm::vec3 spherePos, float sphereRadius, bool sphereCollision)
{
	for (int i = 0; i < currentParticles; i++)
	{
		glm::vec3 distance = pos[i] - spherePos;

		// -- EULER METHOD IF COLLISION
		// - WALLS
		// Left X plane
		if (pos[i].x < -5.f) {}
		// Right X plane
		else if (pos[i].x > 5.f) {}
		// Up Y plane
		else if (pos[i].y > 10.f) {}
		// Down Y plane
		else if (pos[i].y < 0.1f)
		{
			pos[i] = pos[i] - (1 + elastCoef) * (glm::dot(glm::normalize(glm::cross(glm::vec3(-5, 0, 5), glm::vec3(5, 0, 5))), pos[i] + (pos[i].y - 0.1f))) * glm::normalize(glm::cross(glm::vec3(-5, 0, 5), glm::vec3(5, 0, 5)));
			vel[i] = vel[i] - (1 + elastCoef) * (glm::dot(glm::normalize(glm::cross(glm::vec3(-5, 0, 5), glm::vec3(5, 0, 5))), vel[i])) * glm::normalize(glm::cross(glm::vec3(-5, 0, 5), glm::vec3(5, 0, 5)));

			// Trying to implement friction
			//glm::vec3 vN = (glm::dot(glm::cross(glm::vec3(-5, 0, 5), glm::vec3(5, 0, 5)), vel[i])) * glm::normalize(glm::cross(glm::vec3(-5, 0, 5), glm::vec3(5, 0, 5)));
			//glm::vec3 vT = vel[i] - vN;

			//vel[i] = vel[i] - friccCoef * vT;
		}
		// Back Z plane
		else if (pos[i].z < -5.f) {}
		// Forward Z plane
		else if (pos[i].z > 5.f) {}

		// Sphere detection
		else if ((sqrt(pow(distance.x, 2) + pow(distance.y, 2) + pow(distance.z, 2)) < sphereRadius) && (sphereCollision)) {}

		// -- EULER METHOD IF NO COLLISION
		else
		{
			pos[i] += vel[i] * dt;
			vel[i] += acc[i] * dt;
		}

	}
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
