#include <imgui\imgui.h>
#include <imgui\imgui_impl_sdl_gl3.h>
#include <glm\glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "ParticleSystem.h"
#include "Mesh.h"
#include "Solver.h"

#include <ctime>

// Forward declaration
extern bool renderParticles;
extern bool renderSphere;
extern bool renderCapsule;
extern bool renderCloth;

namespace Sphere {
	extern void updateSphere(glm::vec3 pos, float radius);
}

// ParticleSystem variables
ParticleSystem ps;
bool simulationIsActive;
Mesh mesh; // cloth
Verlet solver; // movement

// Sphere gui variablee modifier
glm::vec3 spherePos = glm::vec3(0.f, 1.f, 0.f);
float sphereRadius = 1.f;

// Forces
glm::vec3* forces;
glm::vec3 *gravity;

// Spring parameters for GUI	--> NO BEING USED YET
float k_stretch_parameters[] = {30.f, 1.f};
float k_shear_parameters[] = { 60.f, 1.f };
float k_bend_parameters[] = { 90.f, 1.f };
float linkDistance = 0.2f;

// Times
clock_t start, diff;
float elapsedsec = 0.0f;
float sec = 10.f;
bool once = false;


bool show_test_window = false;
void GUI() {
	bool show = true;
	ImGui::Begin("Physics Parameters", &show, 0);
	{
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);//FrameRate

		ImGui::Checkbox("Use Particles", &renderParticles);
		ImGui::Checkbox("Use Cloth", &renderCloth);
		ImGui::Checkbox("Activate Simulation", &simulationIsActive);

		ImGui::DragFloat("Timer", &sec, 0.01f);

		if (ImGui::CollapsingHeader("Collisions"))
		{
			ImGui::Checkbox("Use Sphere", &renderSphere);
			ImGui::DragFloat3("Sphere Position", glm::value_ptr(spherePos), 0.01f);
			ImGui::DragFloat("Sphere Radius", &sphereRadius, 0.01f);
		}

		if (ImGui::CollapsingHeader("Forces"))
		{
			ImGui::Checkbox("Activate Forces", &mesh.isForcesActivated);
			ImGui::DragFloat3("Gravity", glm::value_ptr(*gravity), 0.01f);
		}
		
		if (ImGui::CollapsingHeader("Springs"))
		{
			ImGui::DragFloat2("Stretch", k_stretch_parameters, 0.01f);
			ImGui::DragFloat2("Shear", k_shear_parameters, 0.01f);
			ImGui::DragFloat2("Bend", k_bend_parameters, 0.01f);
			
			ImGui::DragFloat("Particle Link Distance", &linkDistance, 0.01f, 0.0f, 1.0f);
		}
	}

	ImGui::End();
}

void ResetCloth()
{
	//mesh.Reset();
	
	mesh = Mesh(ClothMesh::numCols, ClothMesh::numRows, linkDistance);
	mesh.CreateSprings(linkDistance, k_stretch_parameters, k_shear_parameters, k_bend_parameters);
}

void PhysicsInit() {

	// Render prims
	renderSphere = false;
	renderParticles = true;
	renderCloth = true;

	// Init solver
	solver = Verlet();
	
	// Init Mesh & Springs
	mesh = Mesh(ClothMesh::numCols, ClothMesh::numRows, linkDistance);
	mesh.CreateSprings(linkDistance, k_stretch_parameters, k_shear_parameters, k_bend_parameters);
	
	// Init Particles
	LilSpheres::particleCount = mesh.width * mesh.height;
	ps = ParticleSystem(LilSpheres::particleCount);
	
	// Init Forces
	forces = new glm::vec3[mesh.maxParticles];
	gravity = new glm::vec3[mesh.maxParticles];
	for (int i = 0; i < mesh.maxParticles; i++)
	{
		gravity[i] = glm::vec3(0.0f, -9.81f, 0.0f);
	}
}

void PhysicsUpdate(float dt) {
		
	if (simulationIsActive)
	{	
		if (!once)
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
			ResetCloth();
			elapsedsec = 0.0f;
			start = clock();
		}

		forces = mesh.get_spring_forces();
		//
		//if (mesh.isForcesActivated)
		//{
		//	for (int i = 0; i < mesh.maxParticles; i++)
		//	{
		//		forces[i] += gravity[i];
		//	}
		//}

		solver.UpdateParticles(mesh, forces, dt);
	}

	ClothMesh::updateClothMesh(&(mesh.pos[0].x));
	LilSpheres::updateParticles(0, mesh.width * mesh.height, &(mesh.pos[0].x));
	Sphere::updateSphere(spherePos, sphereRadius);
}

void PhysicsCleanup() {
}