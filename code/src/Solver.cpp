#include "Solver.h"

void SIEuler::Update(RigidBody* rb, glm::vec3 forces, glm::vec3 torques, float dt)
{
	rb->state.linearMomentum += dt * forces;	// Linear momemntum update
	rb->state.angularMomentum += dt * torques;	// Angular momentum update
	
	glm::vec3 velL = rb->state.linearMomentum / rb->getMass();	// Linear velocity
	rb->state.com += dt * velL; // Center of Mass. Position update
	
	glm::vec3 velA = glm::inverse(rb->getInertiaTensor()) * rb->state.angularMomentum; // Angular velocity
	glm::quat rotation_der = (1.f / 2.f) * velA * glm::normalize(rb->state.rotation);	// Rotation derived

	glm::quat rot = rb->state.rotation + dt * rotation_der;
	
	rb->state.rotation = glm::normalize(rot); // Rotation update
}