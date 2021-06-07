
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

	float heightSubmerged = 0.f;

	Ball() {};
	Ball(glm::vec3 _pos, glm::vec3 _vel, float _r, float _mass) : pos(_pos), vel(_vel), radius(_r), mass(_mass) {};
	
	float volume_total() { return (4.f / 3.f) * glm::pi<float>() * glm::pow(radius, 3); }; // Total volume sphere
	float volume_spherical_cap(float height) { return ((glm::pi<float>() * glm::pow(height, 2)) / 3.f) * ( (3.f * radius) - height); }; // formule from: https://ca.wikipedia.org/wiki/Casquet_esf%C3%A8ric 
};