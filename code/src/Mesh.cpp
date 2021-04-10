#include "Mesh.h"

Mesh::Mesh() 
{
	kElasticity = 1.0f;
	kDamping = 0.0f;;
}

Mesh::Mesh(int w, int h) : width(w), height(h), ParticleSystem(w* h)
{
	kElasticity = 1.0f;
	kDamping = 0.0f;;

	float z = 0.0f;
	
	for (int row = 0; row < h; row++)
	{
		for (int col = 0; col < w; col++)
		{
			pos[get_index(row, col)] = glm::vec3(row, 5.0f, col);
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
	// NO DAMPING
	// F = -k_Elasticity * (p1 - p2)

	glm::vec3 pF = p1 - p2;
	
	return -k_Elasticity * (glm::length(pF) - rest_Distance) * (p1 - p2 / glm::length(pF));
}

glm::vec3* Mesh::get_spring_forces()
{
	// AQUI HAY QUE CALCULAR LA FUERZA DE UN ÚNICO MUELLE RESPECTO A SUS TRES PARÁMETROS 
	// Spring stiffness (elasticity)
	// Damping
	// Original Spring length

	return nullptr;
}