#include "Mesh.h"

Mesh::Mesh(){}

Mesh::Mesh(int w, int h, float linkDistance) : width(w), height(h), ParticleSystem(w* h)
{
	kElasticity = 30.f;
	kDamping = 1.f;
	
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
	//glm::vec3* positiveSpringForces = new glm::vec3[springs.size()];
	//glm::vec3* negateSpringForces = new glm::vec3[springs.size()];

	//// Init to 0
	//for (int i = 0; i < springs.size(); i++)
	//{
	//	positiveSpringForces[i] = glm::vec3(0.f);
	//	negateSpringForces[i] = glm::vec3(0.f);
	//}

	//for (int i = 0; i < springs.size(); i++)
	//{
	//	positiveSpringForces[i] = springs[i].get_p1_force(pos, pos, vel, vel);
	//	negateSpringForces[i] = springs[i].get_p2_force(pos, pos, vel, vel);
	//}

	//for (int i = 0; i < maxParticles; i++)
	//{
	//	for (int j = 0; j < springs.size(); j++)
	//	{
	//		// POR CADA PARTÍCULA. [MAX PARTICLES = 272 MAS O MENOS]
	//		// POR CADA MUELLE EN EL QUE SU ÍNDICE SEA IGUAL AL NÚMERO DE LA PARTÍCULA. [400 Y ALGO] 
	//		if (i == springs[i].GetP1Index())
	//		{
	//			forces[i] += positiveSpringForces[j];
	//			//forces[i + 1] += negateSpringForces[j];
	//		}
	//	}
	//}

	for (int i = 0; i < maxParticles; i++)
	{
		forces[i] += springs[i].get_p2_force(pos, pos, vel, vel);
		forces[i + 1] += springs[i].get_p2_force(pos, pos, vel, vel);
	}

	return forces;
}

void Mesh::CreateSprings()
{
#pragma region Structural Strings
	// LEFT - RIGHT
	for (int row = 0; row < height; row++)
	{
		for (int col = 0; col < width - 1; col++)
		{
			springs.push_back(Spring(kElasticity, kDamping, 2.f, get_index(row, col), get_index(row, col) + 1));
		}
	}
	int x = 0;
	// DOWN - UP
	for (int col = 0; col < width; col++)
	{
		for (int row = 0; row < height - 1; row++)
		{
			springs.push_back(Spring(kElasticity, kDamping, 2.f, get_index(row, col), get_index(row, col) + 1));
		}
	}

	x = 0;
#pragma endregion

#pragma region Shear
	// DIAGONAL
	//for (int row = 0; row < height; row++)
	//{
	//	for (int col = 0; col < width - 1; col++)
	//	{
	//		springs.push_back(Spring(kElasticity, kDamping, 2.f, get_index(row, col), get_index(row, col) + width + 1));
	//	}
	//}

#pragma endregion

#pragma region Bending

	//// LEFT-RIGHT
	//for (int row = 0; row < height; row++)
	//{
	//	for (int col = 0; col < width - 1; col++)
	//	{
	//		springs.push_back(Spring(kElasticity, kDamping, 2.f, get_index(row, col), get_index(row, col) + 2));
	//	}
	//}
	//// DOWN - UP
	//for (int col = 0; col < width; col++)
	//{
	//	for (int row = 0; row < height - 1; row++)
	//	{
	//		springs.push_back(Spring(kElasticity, kDamping, 2.f, get_index(row, col), get_index(row, col) + 2));
	//	}
	//}

#pragma endregion
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