#include "Fluids.h"

Fluids::Fluids() {}

Fluids::Fluids(int w, int h, float linkDistance) : width(w), height(h), ParticleSystem(w* h)
{

	for (int row = 0; row < h; row++)
	{
		for (int col = 0; col < w; col++)
		{
			pos[get_index(row, col)] = glm::vec3((col * linkDistance) - 2, 2.f, (row * linkDistance) - 3.f);
			initialPos[get_index(row, col)] = pos[get_index(row, col)];
		}
	}
}

int Fluids::get_index(int row, int col) {
	return row * width + col;
}

void Fluids::Draw()
{
	ClothMesh::updateClothMesh(&(pos[0].x));
}

void Fluids::GerstnerWaves(float dt, std::vector<Wave> wave)
{
	totalTimeExecution += dt;

	for (int i = 0; i < maxParticles; i++)
	{
		pos[i] = initialPos[i] - (get_wave(initialPos[i], wave[0], totalTimeExecution) + get_wave(initialPos[i], wave[1], totalTimeExecution) + get_wave(initialPos[i], wave[2], totalTimeExecution));
		pos[i].y = get_wave_y(initialPos[i], wave[0], totalTimeExecution) + get_wave_y(initialPos[i], wave[1], totalTimeExecution) + get_wave_y(initialPos[i], wave[2], totalTimeExecution);
	}
}

glm::vec3 Fluids::get_wave(glm::vec3 pos, Wave wave, float dt)
{
	float waveLength = (2 * glm::pi<float>()) / wave.distWaves;

	return (wave.dir / waveLength) * wave.amplitude * sinf(glm::dot(wave.dir, pos) - (wave.velocity * dt));
}
float Fluids::get_wave_y(glm::vec3 pos, Wave wave, float dt)
{
	return wave.amplitude * glm::cos(glm::dot(wave.dir, pos) - (wave.velocity * dt));;
}

void Fluids::Reset()
{
	delete pos;
	delete initialPos;
}