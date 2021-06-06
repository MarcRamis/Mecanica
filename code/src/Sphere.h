
#include <glm\glm.hpp>
#include <glm\gtc\type_ptr.hpp>

namespace Sphere {
	extern void updateSphere(glm::vec3 pos, float radius);
}

class Ball {
public:
	glm::vec3 pos;
	glm::vec3 vel;
	float mass;
	float radius;

	Ball() {};
	Ball(glm::vec3 _pos, glm::vec3 _vel, float _r, float _mass) : pos(_pos), vel(_vel), radius(_r), mass(_mass) {};
};