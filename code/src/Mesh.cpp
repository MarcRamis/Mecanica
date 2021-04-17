#include "Mesh.h"

Mesh::Mesh(){}

Mesh::Mesh(int w, int h) : width(w), height(h), ParticleSystem(w* h)
{
	kElasticity = 500.f;
	kDamping = 0.f;

	float z = 0.0f;
	
	for (int row = 0; row < h; row++)
	{
		for (int col = 0; col < w; col++)
		{
			pos[get_index(row, col)] = glm::vec3(row * 5.f, 5.0f, col);
		}
	}
}

int Mesh::get_index(int row, int col) {
	return row * width + col;	// HACEMOS ESTO PARA PASAR DE FILA COLUMNA DE LA MESH A LA PART�CULA QUE LE TOCA
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
	
	return -(k_Elasticity * (glm::length(pF) - rest_Distance) + k_Damping * (glm::dot((vF), glm::normalize(pF)) )) * glm::normalize(pF);
}

glm::vec3* Mesh::get_spring_forces()
{
	// AQUI HAY QUE CALCULAR LA FUERZA DE UN �NICO MUELLE RESPECTO A SUS TRES PAR�METROS 
	// Spring stiffness (elasticity)
	// Damping
	// Original Spring length

	return nullptr;
}