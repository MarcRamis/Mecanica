#include "Fluids.h"

Fluids::Fluids(int w, int h) : width(w), height(h)
{
	pos = new glm::vec3[w * h];
	float linkDistance = 0.35f;
	
	for (int row = 0; row < h; row++)
	{
		for (int col = 0; col < w; col++)
		{
			pos[get_index(row, col)] = glm::vec3((col * linkDistance) - 2, 5.f, (row * linkDistance) - 2);
		}
	}
}

int Fluids::get_index(int row, int col) {
	return row * width + col;	// HACEMOS ESTO PARA PASAR DE FILA COLUMNA DE LA MESH A LA PARTÍCULA QUE LE TOCA
}

void Fluids::Draw()
{
	ClothMesh::updateClothMesh(&(pos[0].x));
}