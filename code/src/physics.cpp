#include <imgui\imgui.h>
#include <imgui\imgui_impl_sdl_gl3.h>
#include <glm\glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "ParticleSystem.h"

// Forward declaration
extern bool renderParticles;
extern bool renderSphere;
extern bool renderCapsule;

namespace Sphere {
	extern void updateSphere(glm::vec3 pos, float radius);
}

namespace Capsule {
	extern void updateCapsule(glm::vec3 posA, glm::vec3 posB, float radius);
}

// ParticleSystem variables
ParticleSystem ps;
bool simulationIsActive;

// Sphere gui variablee modifier
glm::vec3 spherePos = glm::vec3(0.f, 1.f, 0.f);
float sphereRadius = 1.f;

// Capsule gui variable modifier
glm::vec3 capsulePosA = glm::vec3(-3.f, 2.f, -2.f);
glm::vec3 capsulePosB = glm::vec3(-4.f, 2.f, 2.f);
float capsuleRadius = 1.f;

bool show_test_window = false;
void GUI() {
	bool show = true;
	ImGui::Begin("Physics Parameters", &show, 0);
	{	
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);//FrameRate
		
		ImGui::Checkbox("Activate Simulation", &simulationIsActive);

		if (ImGui::TreeNode("Emitter"))
		{
			ImGui::SliderFloat("Emission rate", &ps.emisisonRate, 10, 1500);
			ImGui::SliderFloat("Particle life", &ps.maxAge, 0.033f, 10.f);
			
			ImGui::RadioButton("Fountain", &ps.type, 0); ImGui::SameLine();
			ImGui::RadioButton("Cascade", &ps.type, 1);

			if (ps.type == 0)
			{
				ImGui::DragFloat3("Fountain Position", glm::value_ptr(ps.f_PosOrigen), 0.01f);
				ImGui::DragFloat3("Fountain Direction", glm::value_ptr(ps.f_Dir), 0.01f);
			}
			
			if (ps.type == 1)
			{
				ImGui::DragFloat3("PointA", glm::value_ptr(ps.pointA), 0.01f);
				ImGui::DragFloat3("PointB", glm::value_ptr(ps.pointB), 0.01f);
				ImGui::DragFloat3("Cascade Direction", glm::value_ptr(ps.c_Dir), 0.01f);
			}
			
			ImGui::SliderFloat("Elasticity", &ps.elastCoef, 0, 1);

			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Sphere"))
		{
			ImGui::Checkbox("Use Sphere", &renderSphere);
			ImGui::DragFloat3("Sphere Position", glm::value_ptr(spherePos), 0.01f);
			ImGui::DragFloat("Sphere Radius", &sphereRadius, 0.01f);

			ImGui::TreePop();
		}
		
		if (ImGui::TreeNode("Capsule"))
		{
			ImGui::Checkbox("Use Capsule", &renderCapsule);
			ImGui::DragFloat3("Capsule Position A", glm::value_ptr(capsulePosA), 0.01f);
			ImGui::DragFloat3("Capsule Position B", glm::value_ptr(capsulePosB), 0.01f);
			ImGui::DragFloat("Capsule Radius", &capsuleRadius, 0.01f);

			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Forces"))
		{
			ImGui::Checkbox("Activate Forces", &ps.isForcesActivated);
			ImGui::DragFloat3("Gravity", glm::value_ptr(ps.gravity), 0.01f);
			
			ImGui::TreePop();
		}
	}
	
	ImGui::End();
}

void PhysicsInit() {
	
	// Prims
	renderParticles = true;
	renderSphere = false;
	renderCapsule = false;

	ps = ParticleSystem(17000);
}

void EmitterFountain(float dt)
{
	float randX = static_cast <float>(rand() / 15.f);
	float randZ = static_cast <float>(rand() / 15.f);

	ps.SpawnParticle(ps.f_PosOrigen, glm::vec3(ps.f_Dir.x * sin(randX), ps.f_Dir.y * cos(0), ps.f_Dir.z * sin(randZ)), dt);
}

void EmitterCascade(float dt)
{
	ps.randomSpawn.x = ps.pointB.x - ps.pointA.x;
	ps.randomNum = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / ps.randomSpawn.x));
	ps.randomSpawn.x = ps.randomNum + ps.pointB.x;

	ps.randomSpawn.y = ps.pointB.y - ps.pointA.y;
	ps.randomNum = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / ps.randomSpawn.y));
	ps.randomSpawn.y = ps.randomNum + ps.pointB.y;

	ps.randomSpawn.z = ps.pointB.z - ps.pointA.z;
	ps.randomNum = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / ps.randomSpawn.z));
	ps.randomSpawn.z = ps.randomNum + ps.pointB.z;
	
	ps.SpawnParticle(ps.randomSpawn, ps.c_Dir, dt);
}

void Spawn(float dt)
{
	if (ps.type == 0)
	{
		EmitterFountain(dt);
	}
	else if (ps.type == 1)
	{
		EmitterCascade(dt);
	}
}

void PhysicsUpdate(float dt) {

	ps.DestroyOldParticles(ps.maxAge);

	if (simulationIsActive)
	{
		if (ps.GetCurrentParticles() < ps.GetMaxParticles())
		{
			for (int i = 0; i < ps.emisisonRate * dt; i++)
			{
				Spawn(dt);
			}
		}

		ps.Update(dt, spherePos, sphereRadius, renderSphere);
	}

	ps.UpdateAge(dt);

	// Update Prims
	ps.UpdateLilSpheres();
	Sphere::updateSphere(spherePos, sphereRadius);
	Capsule::updateCapsule(capsulePosA, capsulePosB, capsuleRadius);
}

void PhysicsCleanup() {
}