#include <imgui\imgui.h>
#include <imgui\imgui_impl_sdl_gl3.h>

#include <glm/glm.hpp>

#include <ctime>

#pragma region Forward Declarations

extern bool renderCloth;

namespace ClothMesh {
	extern void updateClothMesh(float* array_data);
	extern const int numCols;
	extern const int numRows;
}

namespace Sphere {
	extern void updateSphere(glm::vec3 pos, float radius);
}

#pragma endregion

#pragma region Variables

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
}

void PhysicsUpdate(float dt) {

	if (activateSimulation)
	{
		Timer();
	}
}

void PhysicsCleanup() {
}

#pragma endregion