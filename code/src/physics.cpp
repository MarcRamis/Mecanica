#include <imgui\imgui.h>
#include <imgui\imgui_impl_sdl_gl3.h>
#include <glm\glm.hpp>
#include <glm\gtc\quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

#include <iostream>

#include "Rigidbody.h"
#include "Solver.h"

#pragma region Forward Declarations

extern bool renderCube;
extern bool renderSphere;

namespace Gravity {
	extern glm::quat getRotationQuaternion(glm::vec3 axis, float angle);
	extern glm::vec3 getGravityForce(RigidBody* r1, RigidBody* r2);
	extern void init();
	extern void update(float dt);
	extern void cleanup();
}

float G = 0.001f;

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

#pragma endregion

#pragma region Variables

glm::vec3 gravity = glm::vec3(0.f, 0.f,0.f);
glm::vec3 torque = glm::vec3(0.f, 0.f, 1.f);

#pragma endregion

#pragma region ImGui

bool show_test_window = false;
void GUI() {
	bool show = true;
	ImGui::Begin("Physics Parameters", &show, 0);

	{	
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);//FrameRate
		
		ImGui::DragFloat3("Gravity",glm::value_ptr(gravity),0.1f);
		ImGui::DragFloat3("Torque", glm::value_ptr(torque), 0.1f);
	}
	
	ImGui::End();
}

#pragma endregion

#pragma region Loop Physics

Box* box;
Ball* ball;
SIEuler solver;

void PhysicsInit() {

	box = new Box(1.f,1.f,1.f,1.f);

	box->initializeState(
		glm::vec3(0.f, 5.f, 0.f),
		glm::quat(0.f,0.f,0.f,0.f),
		glm::vec3(0.f, 0.f, 0.f), 
		glm::vec3(0.f, 0.f, 0.f));
	
	renderCube = true;
}

void PhysicsUpdate(float dt) {
	
	glm::vec3 force = glm::vec3(gravity);
	glm::vec3 torques = glm::vec3(torque);

	solver.Update(box, force, torques, dt);
	
	box->draw();
}

void PhysicsCleanup() {
	delete box;
}

#pragma endregion