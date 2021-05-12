#pragma once
#include <glm\glm.hpp>

#include "RigidBody.h"

class Solver
{
public:
	virtual void Update(glm::vec3 *pos, glm::vec3 *vel, float dt) {};
};

class SIEuler : public Solver
{
public:

	void Update(RigidBody *rb, glm::vec3 forces, glm::vec3 torques, float dt);
};