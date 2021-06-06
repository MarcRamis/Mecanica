#include <imgui\imgui.h>
#include <imgui\imgui_impl_sdl_gl3.h>

#include <glm/glm.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtx\string_cast.hpp>

#include <ctime>
#include <iostream>
#include <vector>

#include "Fluids.h"
#include "Solver.h"

#pragma region Forward Declarations

extern bool renderCloth;
extern bool renderParticles;
extern bool renderSphere;

#pragma endregion

#pragma region Variables

ParticleSystem ps;
Fluids fluids;
Ball sphere;
Euler solver;

bool activateSimulation = false;

// Forces
glm::vec3 gravity = glm::vec3(0.f,-9.81f,0.f);
glm::vec3 normY = glm::vec3(0.f,1.f,0.f);
float massSphere = 20.f;

// Timer
clock_t start, diff;
float elapsedsec = 0.0f;
float sec = 15.f;
bool once = false;

std::vector<Wave> waves = {
	Wave(glm::vec3(1.0f,0.f,0.f),0.8f,0.5f,3.f),
	Wave(glm::vec3(0.0f,0.f,-1.f),0.8f,0.5f,3.f),
	Wave(glm::vec3(1.0f,0.f,1.f),0.8f,0.5f,3.f)
};

#pragma endregion

#pragma region Functions

void ResetFluids()
{
	fluids.Reset();
	fluids = Fluids(ClothMesh::numCols, ClothMesh::numRows);
	sphere = Ball(glm::vec3(0.f, 5.f, 0.f), glm::vec3(0.f, 0.f, 0.f), 1.f, massSphere);
}

void Timer()
{
	if (!once) // DO ONCE
	{
		// Init time
		start = clock();
		once = true;
	}
	// Timer
	diff = clock() - start;
	elapsedsec = diff / CLOCKS_PER_SEC;
	
	if (elapsedsec >= sec)
	{
		ResetFluids();
		elapsedsec = 0.0f;
		start = clock();
	}
}

glm::vec3 getBuoyancyForce(glm::vec3 pos, glm::vec3 gravity, glm::vec3 normY)
{
	return glm::vec3(0.f);
}

#pragma endregion

#pragma region ImGui

bool show_test_window = true;
void GUI() {
	bool show = true;
	ImGui::Begin("Physics Parameters", &show, 0);
	{	
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

		ImGui::Checkbox("Activate Simulation", &activateSimulation);
		ImGui::DragFloat("Timer", &sec, 0.1f);

		if (ImGui::TreeNode("Waves"))
		{
			if (ImGui::TreeNode("Wave 1"))
			{
				ImGui::DragFloat3("Direction", glm::value_ptr(waves[0].dir), 0.1f, -1.f, 1.f);
				ImGui::DragFloat("Amplitude", &waves[0].amplitude, 0.1f);
				ImGui::DragFloat("Distance Between waves", &waves[0].distWaves, 0.1f);
				ImGui::DragFloat("Speed", &waves[0].velocity, 0.1f);

				ImGui::TreePop();
			}
			if (ImGui::TreeNode("Wave 2"))
			{
				ImGui::DragFloat3("Direction", glm::value_ptr(waves[1].dir), 0.1f, -1.f, 1.f);
				ImGui::DragFloat("Amplitude", &waves[1].amplitude, 0.1f);
				ImGui::DragFloat("Distance Between waves", &waves[1].distWaves, 0.1f);
				ImGui::DragFloat("Speed", &waves[1].velocity, 0.1f);

				ImGui::TreePop();
			}
			if (ImGui::TreeNode("Wave 3"))
			{
				ImGui::DragFloat3("Direction", glm::value_ptr(waves[2].dir), 0.1f, -1.f, 1.f);
				ImGui::DragFloat("Amplitude", &waves[2].amplitude, 0.1f);
				ImGui::DragFloat("Distance Between waves", &waves[2].distWaves, 0.1f);
				ImGui::DragFloat("Speed", &waves[2].velocity, 0.1f);

				ImGui::TreePop();
			}

			ImGui::TreePop();
		}

		ImGui::DragFloat3("Gravity", glm::value_ptr(gravity));
	}
	
	if (show_test_window){
		ImGui::ShowTestWindow();
	}

	ImGui::End();
}

#pragma endregion

#pragma region Phyisics Loop

void PhysicsInit() {

	//srand(time(nullptr));
	
	// Init Fluids & Particles
	fluids = Fluids(ClothMesh::numCols, ClothMesh::numRows);
	LilSpheres::particleCount = fluids.width * fluids.height;
	ps = ParticleSystem(LilSpheres::particleCount);
	
	// Init Sphere
	sphere = Ball(glm::vec3(0.f,5.f,0.f), glm::vec3(0.f,0.f,0.f),1.f, massSphere);

	// Render prims
	renderSphere = true;
	renderCloth = true;
	renderParticles = true;
}

void PhysicsUpdate(float dt) {

	if (activateSimulation)
	{
		Timer();
		
		fluids.GerstnerWaves(dt, waves);

		glm::vec3 forces = /*getBuoyancyForce()*/ + gravity;
		solver.UpdateSphere(sphere, gravity,dt);
	}

	fluids.Draw();
	LilSpheres::updateParticles(0, fluids.width * fluids.height, &(fluids.pos[0].x));
	Sphere::updateSphere(sphere.pos,sphere.radius);
}

void PhysicsCleanup() {
}

#pragma endregion