

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

	int width;
	int height;

	Mesh();
	Mesh(int w, int h, float linkDistance);

	void CreateSprings(float linkDistance, float kForces_stretch[], float kForces_shear[], float kForces_bending[]);
	glm::vec3* get_spring_forces();
	void Reset();
};

class Spring
{
private:
	float kElasticity;
	float kDamping;
	float initial_length;

public:
	
	int p1_idx, p2_idx;

	Spring();
	Spring(float k_e, float k_d, float L, int _p1_idx, int _p2_idx)
		: kElasticity(k_e), kDamping(k_d), initial_length(L), p1_idx(_p1_idx), p2_idx(_p2_idx) {};

	glm::vec3 get_p1_force(glm::vec3 *p1, glm::vec3 *p2, glm::vec3 *v1, glm::vec3 *v2);
	glm::vec3 get_p2_force(glm::vec3 *p1, glm::vec3 *p2, glm::vec3 *v1, glm::vec3 *v2);
};