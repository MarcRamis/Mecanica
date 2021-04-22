#include <imgui\imgui.h>
#include <imgui\imgui_impl_sdl_gl3.h>
#include <glm\glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "ParticleSystem.h"
#include "Mesh.h"
#include "Solver.h"

// Forward declaration
extern bool renderParticles;
extern bool renderSphere;
extern bool renderCapsule;
extern bool renderCloth;

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

Mesh mesh;
//Euler solver;
Verlet solver;

glm::vec3* forces;
glm::vec3 gravity = glm::vec3(0.0f, -9.81f, 0.0f);

bool show_test_window = false;
void GUI() {
	bool show = true;
	ImGui::Begin("Physics Parameters", &show, 0);
	{
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);//FrameRate

		ImGui::Checkbox("Activate Simulation", &simulationIsActive);

		if (ImGui::CollapsingHeader("Collisions"))
		{
			ImGui::Checkbox("Use Sphere", &renderSphere);
			ImGui::DragFloat3("Sphere Position", glm::value_ptr(spherePos), 0.01f);
			ImGui::DragFloat("Sphere Radius", &sphereRadius, 0.01f);
		}

		if (ImGui::CollapsingHeader("Forces"))
		{
			ImGui::Checkbox("Activate Forces", &ps.isForcesActivated);
			ImGui::DragFloat3("Gravity", glm::value_ptr(gravity), 0.01f);
		}
	}

	ImGui::End();
}

void PhysicsInit() {
	
	//solver = Euler();
	solver = Verlet();
	
	mesh = Mesh(ClothMesh::numCols, ClothMesh::numRows);
	//renderSphere = false;

	//renderCapsule = false;
	
	renderParticles = true;
	renderCloth = true;
	LilSpheres::particleCount = mesh.width * mesh.height;
	ps = ParticleSystem(LilSpheres::particleCount);
	
	forces = new glm::vec3[mesh.maxParticles];
}

void PhysicsUpdate(float dt) {

	if (simulationIsActive)
	{
		for (int i = 0; i < mesh.maxParticles; i++)
		{
			forces[i] = mesh.spring_force(mesh.kElasticity, mesh.kDamping, 2.f, mesh.pos[i], mesh.pos[i + 1], mesh.vel[i + 1], mesh.vel[i + 1]);;
		}
		//forces[0] = mesh.spring_force(mesh.kElasticity, mesh.kDamping, 2.f, mesh.pos[0], mesh.pos[1], mesh.vel[0], mesh.vel[1]);
		//forces[1] = -forces[0];
		// SUMAR GRAVEDAD 
		//std::cout << "F1: " << glm::to_string(forces[0]) << std::endl << std::endl;

		solver.UpdateParticles(mesh, forces, dt);

	}
	ClothMesh::updateClothMesh(&(mesh.pos[0].x));
	LilSpheres::updateParticles(0, mesh.width * mesh.height, &(mesh.pos[0].x));
}

void PhysicsCleanup() {
}