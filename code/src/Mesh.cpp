#include "Mesh.h"

Mesh::Mesh(){}

Mesh::Mesh(int w, int h) : width(w), height(h), ParticleSystem(w* h)
{
	kElasticity = 30.f;
	kDamping = 1.f;
	
	for (int row = 0; row < h; row++)
	{
		for (int col = 0; col < w; col++)
		{
			pos[get_index(row, col)] = glm::vec3(row, col, 0.0f);
			prevPos[get_index(row, col)] = glm::vec3(row, col, 0.0f);
			//pos[get_index(row, col)] = glm::vec3(row, 5.f, col);
			//prevPos[get_index(row, col)] = glm::vec3(row, 5.f, col);
		}
	}
}

int Mesh::get_index(int row, int col) {
	return row * width + col;	// HACEMOS ESTO PARA PASAR DE FILA COLUMNA DE LA MESH A LA PARTÍCULA QUE LE TOCA
}

glm::vec3 Mesh::spring_force(float k_Elasticity, 
	float k_Damping, 
	float rest_Distance, 
	glm::vec3 p1, glm::vec3 p2,
	glm::vec3 v1, glm::vec3 v2)
{	
	//std::cout << "Elasticity: " << k_Elasticity << std::endl;
	//std::cout << "Damping: " << k_Damping << std::endl;
	//std::cout << "Length: " << rest_Distance << std::endl;
	
	std::cout << "P1: " << glm::to_string(p1) << std::endl;
	std::cout << "P2: " << glm::to_string(p2) << std::endl;
	
	std::cout << "V1: " << glm::to_string(v1) << std::endl;
	std::cout << "V2: " << glm::to_string(v2) << std::endl;
	
	glm::vec3 pF = p1 - p2;
	glm::vec3 vF = v1 - v2;
	glm::vec3 u = glm::normalize(pF);
	
	return -(k_Elasticity * (glm::length(pF) - rest_Distance) + k_Damping * (glm::dot((vF), u) )) * u;
}

glm::vec3* Mesh::get_spring_forces()
{
	// AQUI HAY QUE CALCULAR LA FUERZA DE UN ÚNICO MUELLE RESPECTO A SUS TRES PARÁMETROS 
	// Spring stiffness (elasticity)
	// Damping
	// Original Spring length

	return nullptr;
}

void Mesh::SpringStructures()
{
#pragma region Structural Strings
	// LEFT-RIGHT
	for (int row = 0; row < height; row++)
	{
		for (int col = 0; col < width - 1; col++)
		{
			spr.push_back(Spring(kElasticity, kDamping, 2.f, get_index(row, col), get_index(row, col) + 1));
		}
	}
	// DOWN- UP
	for (int col = 0; col < width; col++)
	{
		for (int row = 0; row < height - 1; row++)
		{
			spr.push_back(Spring(kElasticity, kDamping, 2.f, get_index(row, col), get_index(row, col) + 1));
		}

	}
#pragma endregion

#pragma Shear
	// DIAGONAL
	for (int row = 0; row < height; row++)
	{
		for (int col = 0; col < width - 1; col++)
		{
			spr.push_back(Spring(kElasticity, kDamping, 2.f, get_index(row, col), get_index(row, col) + width + 1));
		}
	}
#pragma endregion

#pragma Bending
	// LEFT-RIGHT
	for (int row = 0; row < height; row++)
	{
		for (int col = 0; col < width - 1; col++)
		{
			spr.push_back(Spring(kElasticity, kDamping, 2.f, get_index(row, col), get_index(row, col) + 2));
		}
	}
	// DOWN - UP
	for (int col = 0; col < width; col++)
	{
		for (int row = 0; row < height - 1; row++)
		{
			spr.push_back(Spring(kElasticity, kDamping, 2.f, get_index(row, col), get_index(row, col) + 2));
		}
	}
#pragma endregion
}

glm::vec3 Spring::spring_force(float k_Elasticity,
	float k_Damping,
	float rest_Distance,
	glm::vec3 p1, glm::vec3 p2,
	glm::vec3 v1, glm::vec3 v2)
{
	glm::vec3 pF = p1 - p2;
	glm::vec3 vF = v1 - v2;
	glm::vec3 u = glm::normalize(pF);

	return -(k_Elasticity * (glm::length(pF) - rest_Distance) + k_Damping * (glm::dot((vF), u))) * u;
}