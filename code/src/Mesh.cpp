#include "Mesh.h"

Mesh::Mesh() {}

Mesh::Mesh(int w, int h) : width(w), height(h), ParticleSystem(w* h)
{
	float z = 0.0f;

	for (int row = 0; row < h; row++)
	{
		for (int col = 0; col < w; col++)
		{
			pos[get_index(row, col)] = glm::vec3(row * 0.2, col * 0.2, z);
		}
	}
}