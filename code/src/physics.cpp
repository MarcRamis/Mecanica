#include <imgui\imgui.h>
#include <imgui\imgui_impl_sdl_gl3.h>

#include <glm/glm.hpp>

#include <ctime>
#include <iostream>

#include "Fluids.h"

#pragma region Forward Declarations

extern bool renderCloth;
extern bool renderSphere;

namespace Sphere {
	extern void updateSphere(glm::vec3 pos, float radius);
}

#pragma endregion

#pragma region Variables

Fluids* fluids;
const int MESH_WIDTH = { 2 };
const int MESH_HEIGHT = { 2 };

bool activateSimulation = true;

// Time
clock_t start, diff;
float elapsedsec = 0.0f;
float sec = 15.f;
bool once = false;

#pragma endregion

#pragma region Functions

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
		//AQUI VENDRÍA EL RESET
		elapsedsec = 0.0f;
		start = clock();
	}
}

#pragma endregion

#pragma region ImGui

bool show_test_window = false;
void GUI() {
	bool show = true;
	ImGui::Begin("Physics Parameters", &show, 0);

	{	
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

		ImGui::Checkbox("Activate Simulation", &activateSimulation);
		ImGui::DragFloat("Timer", &sec, 0.1f);
	}
	
	ImGui::End();
}

#pragma endregion

#pragma region Phyisics Loop

void PhysicsInit() {

	//srand(time(nullptr));
	
	fluids = new Fluids(ClothMesh::numCols, ClothMesh::numRows);
	
	// Render prims
	renderSphere = false;
	renderCloth = true;
}

void PhysicsUpdate(float dt) {

	if (activateSimulation)
	{
		Timer();
	}

	fluids->Draw();
}

void PhysicsCleanup() {
}

#pragma endregion