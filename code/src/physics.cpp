#include <imgui\imgui.h>
#include <imgui\imgui_impl_sdl_gl3.h>
#include <glm\glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "ParticleSystem.h"
#include "Mesh.h"
#include "Solver.h"
#include "Sphere.h"

#include <ctime>

// Forward declaration
extern bool renderParticles;
extern bool renderSphere;
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
SphereCollider sphere(glm::vec3(0.f),1.f);

// Forces
glm::vec3* forces;
glm::vec3 *gravity;
glm::vec3 gravityValues = glm::vec3(0.0f, -9.81f, 0.0f);
bool isForcesActivated = false;

// Spring parameters for GUI	--> NO BEING USED YET
float k_stretch_parameters[] = { 1000.f, 0.1f};
float k_shear_parameters[] = { 1000.f, 0.1f };
float k_bend_parameters[] = { 1000.f, 0.1f };
float linkDistance = 0.3f;

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
			ImGui::Checkbox("Box collision", &solver.enableBox);
			ImGui::Checkbox("Use Sphere", &renderSphere);
			ImGui::DragFloat3("Sphere Position", glm::value_ptr(sphere.pos), 0.01f);
			ImGui::DragFloat("Sphere Radius", &sphere.radius, 0.01f);
		}

		if (ImGui::CollapsingHeader("Forces"))
		{
			ImGui::Checkbox("Activate Forces", &isForcesActivated);
			ImGui::DragFloat3("Gravity", glm::value_ptr(gravityValues), 0.01f);
		}
		
		if (ImGui::CollapsingHeader("Springs"))
		{
			ImGui::DragFloat2("Stretch", k_stretch_parameters, 1.f);
			ImGui::DragFloat2("Shear", k_shear_parameters, 1.f);
			ImGui::DragFloat2("Bend", k_bend_parameters, 1.f);
			
			ImGui::DragFloat("Particle Link Distance", &linkDistance, 0.001f, 0.0f, 1.0f);
		}
	}

	ImGui::End();
}

void ResetCloth()
{
	// Init Cloth
	mesh.Reset(); // this deletes previous mesh
	mesh = Mesh(ClothMesh::numCols, ClothMesh::numRows, linkDistance);
	mesh.CreateSprings(linkDistance, k_stretch_parameters, k_shear_parameters, k_bend_parameters);
	
	// Init Forces
	delete[] forces;
	delete[] gravity;

	forces = new glm::vec3[mesh.maxParticles];
	gravity = new glm::vec3[mesh.maxParticles];
	for (int i = 0; i < mesh.maxParticles; i++)
	{
		gravity[i] = glm::vec3(gravityValues);
	}

	// Init Sphere
	float randX = static_cast <float>((rand() / 5.f) - 5.f);
	float randY = static_cast <float>(rand() / 10.f);
	float randZ = static_cast <float>((rand() / 5.f) - 5.f);

	std::cout << randX << randY << randZ << std::endl;

	sphere.pos = glm::vec3(randX, randY, randZ);
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
		ResetCloth();
		elapsedsec = 0.0f;
		start = clock();
	}
}

void PhysicsInit() {
	
	srand(time(NULL));

	// Render prims
	renderSphere = false;
	renderParticles = true;
	renderCloth = true;

	// Init solver
	solver = Verlet();
	
	// Init Mesh & Springs
	mesh = Mesh(ClothMesh::numCols, ClothMesh::numRows, linkDistance);
	isForcesActivated = false;
	// PRINT PARTICLES IN 2D FORMAT
	for (int row = 0; row < mesh.height; row++)
	{
		std::cout << row * mesh.width;
		for (int col = 0; col < mesh.width - 1; col++)
		{
			std::cout << '\t' << row * mesh.width + col + 1;
		}
		std::cout << std::endl;
	}

	mesh.CreateSprings(linkDistance, k_stretch_parameters, k_shear_parameters, k_bend_parameters);
	
	// Init Particles
	LilSpheres::particleCount = mesh.width * mesh.height;
	ps = ParticleSystem(LilSpheres::particleCount);
	
	// Init Forces
	forces = new glm::vec3[mesh.maxParticles];
	gravity = new glm::vec3[mesh.maxParticles];
	for (int i = 0; i < mesh.maxParticles; i++)
	{
		gravity[i] = glm::vec3(gravityValues);
	}
	
	// Init Sphere

	//float randX = static_cast <float>((rand() / (5.f - (-5.f) + 1) + (-5.f)));
	//float randY = static_cast <float>(rand() / 10.f);
	//float randZ = static_cast <float>((rand() / (5.f - (-5.f) + 1) + (-5.f)));

	//std::cout << randY << std::endl;
	//
	//sphere.pos = glm::vec3(randX, randY, randZ);
}

void PhysicsUpdate(float dt) {

	if (simulationIsActive) // IN SIMULATION ON
	{	
		Timer();
		
		for (int i = 0; i < 10; i++)
		{
			forces = mesh.get_spring_forces();

			if (isForcesActivated)
			{
				for (int i = 0; i < mesh.maxParticles; i++)
				{
					forces[i] += gravity[i];
				}
			}

			solver.UpdateParticles(mesh, forces, dt / 10, sphere);
		}
	}

	ClothMesh::updateClothMesh(&(mesh.pos[0].x));
	LilSpheres::updateParticles(0, mesh.width * mesh.height, &(mesh.pos[0].x));
	Sphere::updateSphere(sphere.pos, sphere.radius);
	sphere.enabled = renderSphere;
}

void PhysicsCleanup() {
}