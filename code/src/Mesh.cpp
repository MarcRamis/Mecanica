#include "Mesh.h"

Mesh::Mesh(){}

Mesh::Mesh(int w, int h, float linkDistance) : width(w), height(h), ParticleSystem(w* h)
{
	for (int row = 0; row < h; row++)
	{
		for (int col = 0; col < w; col++)
		{
			pos[get_index(row, col)] = glm::vec3(col * linkDistance, 3.f, row * linkDistance);
			prevPos[get_index(row, col)] = glm::vec3(col * linkDistance, 3.f, row * linkDistance);
		}
	}
}

int Mesh::get_index(int row, int col) {
	return row * width + col;	// HACEMOS ESTO PARA PASAR DE FILA COLUMNA DE LA MESH A LA PARTÍCULA QUE LE TOCA
}

glm::vec3* Mesh::get_spring_forces()
{
	for (int i = 0; i < maxParticles; i++)
	{
		forces[i] = glm::vec3(0.f, 0.f, 0.f);
	}

	for (int i = 0; i < maxParticles; i++)
	{
		for (int j = 0; j < springs.size(); j++)
		{
			if (i == springs[j].p1_idx)
			{
				forces[i] += springs[j].get_p1_force(pos, pos, vel, vel);	
			}
			else if (i == springs[j].p2_idx)
			{
				forces[i] += springs[j].get_p2_force(pos, pos, vel, vel);
			}
		}
	}

	return forces;
}

void Mesh::CreateSprings(float linkDistance, float kForces_stretch[], float kForces_shear[], float kForces_bending[])
{
#pragma region Structural Strings
	
	// LEFT - RIGHT
	for (int row = 0; row < height; row++)
	{
		for (int col = 0; col < width - 1; col++)
		{
			springs.push_back(Spring(kForces_stretch[0], kForces_stretch[1], linkDistance, get_index(row, col), get_index(row, col + 1)));
		}
	}

	// DOWN - UP
	for (int row = 0; row < height - 1; row++)
	{
		for (int col = 0; col < width; col++)
		{
			springs.push_back(Spring(kForces_stretch[0], kForces_stretch[1], linkDistance, get_index(row, col), get_index(row + 1, col)));
		}
	}
	
#pragma endregion

#pragma region Shear
	// RIGHT DIAGONAL
	for (int row = 0; row < height - 1; row++)
	{
		for (int col = 0; col < width - 1; col++)
		{
			springs.push_back(Spring(kForces_shear[0], kForces_shear[1],
				glm::sqrt(glm::pow(linkDistance,2) + glm::pow(linkDistance, 2)), get_index(row, col), get_index(row, col + width + 1)));
		}
	}

	// LEFT DIAGONAL
	for (int col = width - 1; col > 0; col--)
	{
		for (int row = 0; row < height - 1; row++)
		{
			springs.push_back(Spring(kForces_shear[0], kForces_shear[1],
				glm::sqrt(glm::pow(linkDistance, 2) + glm::pow(linkDistance, 2)), get_index(row, col), get_index(row, col + width - 1)));
		}
	}
#pragma endregion

#pragma region Bending
	// LEFT - RIGHT
	for (int row = 0; row < height; row++)
	{
		for (int col = 0; col < width - 2; col++)
		{
			springs.push_back(Spring(kForces_bending[0], kForces_bending[1], linkDistance, get_index(row, col), get_index(row, col + 2)));
		}
	}

	// DOWN - UP
	for (int row = 0; row < height - 2; row++)
	{
		for (int col = 0; col < width; col++)
		{
			springs.push_back(Spring(kForces_bending[0], kForces_stretch[1], linkDistance, get_index(row, col), get_index(row + 2, col)));
		}
	}

#pragma endregion
}

void Mesh::Reset()
{
	springs.clear();
	delete[] pos;
	delete[] prevPos;
	delete[] vel;
	maxParticles = 0;
}

glm::vec3 Spring::get_p1_force(glm::vec3 *_p1, glm::vec3 *_p2, glm::vec3 *_v1, glm::vec3 *_v2)
{
	glm::vec3 p1 = _p1[p1_idx];
	glm::vec3 p2 = _p2[p2_idx];
	glm::vec3 v1 = _v1[p1_idx];
	glm::vec3 v2 = _v2[p2_idx];

	glm::vec3 pF = p1 - p2;
	glm::vec3 vF = v1 - v2;
	glm::vec3 u = glm::normalize(pF);
	
	return -(kElasticity * (glm::length(pF) - initial_length) + kDamping * (glm::dot((vF), u))) * u;
}

glm::vec3 Spring::get_p2_force(glm::vec3 *p1, glm::vec3 *p2, glm::vec3 *v1, glm::vec3 *v2)
{
	return -get_p1_force(p1, p1, v1, v2);
}