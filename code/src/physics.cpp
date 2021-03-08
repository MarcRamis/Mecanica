#include <imgui\imgui.h>
#include <imgui\imgui_impl_sdl_gl3.h>
#include <glm\glm.hpp>

/////////Forward declarations
namespace LilSpheres {
	extern const int maxParticles;
	extern int firstParticleIdx;
	extern int particleCount;
	extern void updateParticles(int startIdx, int count, float* array_data);
}

extern bool renderParticles;

bool show_test_window = false;
void GUI() {
	bool show = true;
	ImGui::Begin("Physics Parameters", &show, 0);

	{	
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);//FrameRate
	}
	
	ImGui::End();
}

class Particle {
public:
	glm::vec3 position;

	Particle() : position(glm::vec3(0.f, 0.f, 0.f)) {};
	Particle(glm::vec3 _position) : position(_position) {};
};

class ParticleSystem {
public:
	ParticleSystem(int numParticles /*, glm::vec3 *position*/) {};
	
	void updateParticle(int idx, glm::vec3 newPosition) {

	}

	void updateLilSpheres() 
	{
	}
private:
};

Particle p1;

void PhysicsInit() {
	renderParticles = true;
	p1 = Particle(glm::vec3(4.f, 3.f, 2.f));

	LilSpheres::firstParticleIdx = 0;
	LilSpheres::particleCount = 1;
}

void PhysicsUpdate(float dt) {
	int start_particle = 0;
	int number_of_particles = 1;
	float* particles_position_vector = &(p1.position.x);

	//p1.position = glm::vec3(float(rand() / 5.f), float(rand() / 10.f), float(rand() / 5.f));
	//p1.position.x = static_cast<float>(rand() / static_cast <float> (5.f));
	//p1.position.y = static_cast<float>(rand() / static_cast <float> (10.f));
	//p1.position.z = static_cast<float>(rand() / static_cast < float> (5.f));

	//p1.position.x = -5 + p_pars.min + (float)rand() / (RAND_MAX / (p_pars.max - p_pars.min));
	//p1.position.x = p_pars.min + (float)rand() / (RAND_MAX / (p_pars.max - p_pars.min));
	//p1.position.x = -5 + p_pars.min + (float)rand() / (RAND_MAX / (p_pars.max - p_pars.min));

	LilSpheres::updateParticles(start_particle, number_of_particles, particles_position_vector);
}

void PhysicsCleanup() {
}