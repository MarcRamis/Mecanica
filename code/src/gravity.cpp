#include <glm\glm.hpp>
#include <glm\gtc\quaternion.hpp>
#include <glm\gtx\string_cast.hpp>
#include <glm\gtc\matrix_transform.hpp>

#include <iostream>

#include "RigidBody.h"

extern bool renderSphere;
extern bool renderCube;

namespace Gravity {

	Box* box;
	Ball* ball;
	//RigidBodySolver solver;

	const float G = 0.001f;

	glm::quat getRotationQuaternion(glm::vec3 axis, float angle) {
		float w = cos(angle / 2);
		glm::vec3 v = sin(angle / 2) * axis;
		return glm::normalize(glm::quat(w, v));
	}

	glm::vec3 getGravityForce(RigidBody* r1, RigidBody* r2) {
		glm::vec3 direction = r2->getState().com - r1->getState().com;
		float distance = glm::length(direction);
		float magnitude = G * r1->getMass() * r2->getMass() / distance;
		return glm::normalize(direction) * magnitude;
	}

	void init() {
		box = new Box(1.f, 1.f, 2.f, 1.f);
		ball = new Ball(.5f, 1000.f);

		glm::vec3 boxCom = glm::vec3(4.0f, 5.0f, 0.0f);
		glm::vec3 ballCom = glm::vec3(0.f, 5.f, 0.f);

		glm::vec3 boxLinearVelocity = glm::vec3(0.f, 0.f, 1.f);

		box->initializeState(
			boxCom,
			getRotationQuaternion(glm::vec3(1.f, 1.f, 0.f), 3.14f / 2.f),
			boxLinearVelocity,
			glm::vec3(1.0f, 0.0f, 0.0f) // angular velocity
		);
		ball->initializeState(glm::vec3(0.f, 5.f, 0.f), glm::vec3(0.f), glm::vec3(0.f), glm::vec3(0.f));

		renderCube = true;
		renderSphere = true;
	}

	void update(float dt) {
		glm::vec3 force = getGravityForce(box, ball);
		glm::vec3 torques = glm::vec3(0.f);

		//solver.updateState(box, force, torques, dt);
		//solver.updateState(ball, -force, torques, dt);

		box->draw();
		ball->draw();
	}

	void cleanup() {
		delete box;
		delete ball;
	}
}