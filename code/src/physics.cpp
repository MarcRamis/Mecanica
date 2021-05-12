#include <imgui\imgui.h>
#include <imgui\imgui_impl_sdl_gl3.h>
#include <glm\glm.hpp>
#include <glm\gtc\quaternion.hpp>

#include "Rigidbody.h"

#pragma region Forward Declarations

namespace Cube {
	extern void updateCube(const glm::mat4& transform);
}

namespace Sphere {
	extern void updateSphere(glm::vec3 pos, float radius = 1.f);
}
extern bool renderCube;
extern bool renderSphere;

namespace Gravity {
	extern glm::quat getRotationQuaternion(glm::vec3 axis, float angle);
	extern glm::vec3 getGravityForce(RigidBody* r1, RigidBody* r2);
	extern void init();
	extern void update(float dt);
	extern void cleanup();
}

#pragma endregion

#pragma region ImGui

bool show_test_window = false;
void GUI() {
	bool show = true;
	ImGui::Begin("Physics Parameters", &show, 0);

	{	
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);//FrameRate
	}
	
	ImGui::End();
}

#pragma endregion

#pragma region Loop Physics
Box* box;
Ball* ball;

void PhysicsInit() {

	box = new Box(1.f,1.f,1.f,1.f);
	ball = new Ball(1.f,1.f);

	box->initializeState(
		glm::vec3(0.f, 5.f, 0.f), 
		glm::quat(0.f, 0.f, 0.f, 0.f), 
		glm::vec3(0.f, 0.f, 0.f), 
		glm::vec3(0.f, 0.f, 0.f));

	ball->initializeState(
		glm::vec3(2.5f, 5.f, 0.f),
		glm::quat(0.f, 0.f, 0.f, 0.f),
		glm::vec3(0.f, 0.f, 0.f),
		glm::vec3(0.f, 0.f, 0.f));

	renderCube = true;
	renderSphere = true;
}

void PhysicsUpdate(float dt) {

	box->draw();
	ball->draw();
}

void PhysicsCleanup() {
	delete box;
}

#pragma endregion