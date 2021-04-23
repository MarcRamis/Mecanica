

#include "ParticleSystem.h"

class Spring;

namespace ClothMesh {
	extern void updateClothMesh(float* array_data);
	extern const int numCols;
	extern const int numRows;
}

class Mesh : public ParticleSystem
{
private:
	
	std::vector<Spring> spr;
	
public:
	int get_index(int row, int col);

	float kElasticity;
	float kDamping;
	float length;

	int width;
	int height;

	Mesh();
	Mesh(int w, int h);

	glm::vec3 spring_force(float kE, float kD, float rest_Distance, glm::vec3 p1, glm::vec3 p2, glm::vec3 v1, glm::vec3 v2);
	glm::vec3* get_spring_forces();

	void SpringStructures();
};

class Spring
{
private:
	float kElasticity;
	float kDamping;
	float initial_length;
	int p1, p2;

public:

	Spring();
	Spring(float k_e, float k_d, float L, int _p1, int _p2)
		: kElasticity(k_e), kDamping(k_d), initial_length(L), p1(_p1), p2(_p2) {};

	glm::vec3 spring_force(float kE, float kD, float rest_Distance, glm::vec3 p1, glm::vec3 p2, glm::vec3 v1, glm::vec3 v2);
};