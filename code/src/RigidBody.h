#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include <iostream>
#include <glm/gtx/string_cast.hpp>

#pragma once
class RigidBody {
public:
	struct State {
		glm::vec3 com;  // Position of the Center Of Mass
		glm::quat rotation;  // Quaternion that represents the current rotation q(t)
		glm::vec3 linearMomentum;  // P(t)
		glm::vec3 angularMomentum;  // L(t)

		State() {};
		State(glm::vec3 _com, glm::quat _rot, glm::vec3 _linearMomentum, glm::vec3 _angularMomentum) 
			: com(_com), rotation(_rot), linearMomentum(_linearMomentum), angularMomentum(_angularMomentum) {}
	};

	RigidBody(float mass);
	void initializeState(glm::vec3 initialPosition, glm::quat initialRotation, glm::vec3 linearSpeed, glm::vec3 angularSpeed);

	State getState();
	void setState(State state);
	State rollbackState();
	void commitState();

	float getMass();
	glm::mat3 getInertiaTensor();
	virtual glm::mat3 getInitialInertiaTensor() = 0;
	virtual void draw() = 0;
	
	glm::mat3 getRotationMatrix();

	State state;
protected:
	
	
private:
	float mass;
	glm::mat3 initialInertiaTensor;
	State stableState;
};

class Box : public RigidBody {
public:
	Box(float width, float height, float depth, float mass);
	virtual void draw() override;

protected:
	virtual glm::mat3 getInitialInertiaTensor() override;
private:
	float width, height, depth;
};