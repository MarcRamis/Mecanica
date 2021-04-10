
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

	glm::vec3 spring_force(float, float, float, glm::vec3, glm::vec3, glm::vec3, glm::vec3);
	glm::vec3* get_spring_forces();
};