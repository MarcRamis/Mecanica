#include <glm/gtx/transform.hpp>

#include "RigidBody.h"

#pragma section forwardDeclarations
namespace Cube {
	extern void updateCube(const glm::mat4& transform);
}
namespace Sphere {
	extern void updateSphere(glm::vec3 pos, float radius = 1.f);
}
#pragma endregion

#pragma section RigidBody
RigidBody::RigidBody(float mass) : mass(mass) {};

void RigidBody::initializeState(glm::vec3 initialPosition, glm::quat initialRotation, glm::vec3 linearSpeed, glm::vec3 angularSpeed) {
	// Initialize the state outside the constructor to use the virtual method getInitialInertiaTensor
	initialInertiaTensor = getInitialInertiaTensor();
	state = {
		initialPosition,
		initialRotation,
		mass * linearSpeed,
		angularSpeed * getInertiaTensor()
	};
}

RigidBody::State RigidBody::getState() {
	return state;
}

void RigidBody::setState(RigidBody::State newState) {
	state = newState;
}

RigidBody::State RigidBody::rollbackState() {
	// If the state is inconsistent, we can go back to the last correct state
	// (for example due to a collision)
	// Return the inconsistent state for cases where we want to use it
	State tmp = state;
	state = stableState;
	return tmp;
}

void RigidBody::commitState() {
	// Convert the state into a stable (correct) state
	stableState = state;
}

float RigidBody::getMass() {
	return mass;
}

glm::mat3 RigidBody::getRotationMatrix() {
	return glm::mat3_cast(state.rotation);
}

glm::mat3 RigidBody::getInertiaTensor() {
	// TODO implement
	return glm::mat3(1.f);
}

#pragma endregion

#pragma region Box
Box::Box(float width, float height, float depth, float mass)
	: RigidBody(mass), width(width), height(height), depth(depth) {};

void Box::draw() {
	RigidBody::State state = getState();
	glm::mat4 transform = glm::translate(glm::mat4(1.f), state.com) *
		glm::mat4_cast(state.rotation) *
		glm::scale(glm::mat4(1.f), glm::vec3(width, height, depth);
	Cube::updateCube(transform);
}

glm::mat3 Box::getInitialInertiaTensor() {
	// TODO implement
	return glm::mat3(1.f);
}
#pragma endregion

#pragma region Ball
Ball::Ball(float radius, float mass) : RigidBody(mass), radius(radius) {};

void Ball::draw() {
	Sphere::updateSphere(getState().com, radius);
}

glm::mat3 Ball::getInitialInertiaTensor() {
	// TODO implement
	return glm::mat3(1.f);
}

#pragma endregion