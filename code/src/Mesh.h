
#include "ParticleSystem.h"

namespace ClothMesh {
	extern void updateClothMesh(float* array_data);
	extern const int numCols;
	extern const int numRows;
}

class Mesh : public ParticleSystem
{
private:


	int get_index(int row, int col);

public:
	float kElasticity;
	float kDamping;
	float length;

	int width;
	int height;

	Mesh();
	Mesh(int w, int h);

	glm::vec3 spring_force(float kE, float kD, float rest_Distance, glm::vec3 p1, glm::vec3 p2, glm::vec3 v1, glm::vec3 v2);
	glm::vec3* get_spring_forces();
};