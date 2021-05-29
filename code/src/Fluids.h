#include <glm\glm.hpp>

#pragma region Forward Declarations

namespace ClothMesh {
	extern void updateClothMesh(float* array_data);
	extern const int numCols;
	extern const int numRows;
}

#pragma endregion

class Fluids
{
public:

	glm::vec3* pos;

	Fluids(int w, int h);
	void Draw();

private:
	
	float width, height;
	int get_index(int row, int col);
};