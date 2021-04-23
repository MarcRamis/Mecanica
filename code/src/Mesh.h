

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
	
	std::vector<Spring> springs;
	int get_index(int row, int col);

public:
	
	float kElasticity;
	float kDamping;
	float length;

	int width;
	int height;

	Mesh();
	Mesh(int w, int h, float linkDistance);

	void CreateSprings();
	glm::vec3* get_spring_forces();
};

class Spring
{
private:
	float kElasticity;
	float kDamping;
	float initial_length;
	int p1_idx, p2_idx;

public:

	Spring();
	Spring(float k_e, float k_d, float L, int _p1_idx, int _p2_idx)
		: kElasticity(k_e), kDamping(k_d), initial_length(L), p1_idx(_p1_idx), p2_idx(_p2_idx) {};

	glm::vec3 get_p1_force(glm::vec3 *p1, glm::vec3 *p2, glm::vec3 *v1, glm::vec3 *v2);
	glm::vec3 get_p2_force(glm::vec3 *p1, glm::vec3 *p2, glm::vec3 *v1, glm::vec3 *v2);

	int GetP1Index() { return p1_idx; };
};