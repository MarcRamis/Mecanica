#include <glm\glm.hpp>
#include <glm\gtc\random.hpp>
#include <glm\gtx\string_cast.hpp>

#include <iostream>

#include "ParticleSystem.h"

#include <vector>

#pragma region Forward Declarations

namespace ClothMesh {
	extern void updateClothMesh(float* array_data);
	extern const int numCols;
	extern const int numRows;
}

#pragma endregion

struct Wave {
	glm::vec3 dir;
	float amplitude;
	float distWaves;
	float velocity;
	
	Wave();
	Wave(glm::vec3 _dir, float _ampl, float _distWaves, float _vel) : dir(_dir), amplitude(_ampl), distWaves(_distWaves), velocity(_vel) {}
};

class Fluids : public ParticleSystem
{
public:

	float width, height;

	Fluids();
	Fluids(int w, int h);
	void Draw();

	void GerstnerWaves(float dt, std::vector<Wave> wave);
	void Reset();

private:
	
	float totalTimeExecution = 0.f;
	glm::vec3 sumWaves = glm::vec3(0.f);
	float sumWavesY = 0.f;

	int get_index(int row, int col);
	
	glm::vec3 get_wave(glm::vec3 pos, Wave wave, float dt);
	float get_wave_y(glm::vec3 pos, Wave wave, float dt);
};