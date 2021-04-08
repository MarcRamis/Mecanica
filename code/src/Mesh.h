
#include "ParticleSystem.h"

namespace ClothMesh {
	extern void updateClothMesh(float* array_data);
	extern const int numCols;
	extern const int numRows;
}

class Mesh : public ParticleSystem
{
private:
	int get_index(int row, int col) {
		return row * width + col;	// HACEMOS ESTO PARA PASAR DE FILA COLUMNA DE LA MESH A LA PARTÍCULA QUE LE TOCA
	}

public:
	int width;
	int height;

	Mesh();
	Mesh(int w, int h);

	glm::vec3* get_spring_forces()
	{
		return nullptr;
	}
};