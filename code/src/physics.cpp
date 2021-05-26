#include <imgui\imgui.h>
#include <imgui\imgui_impl_sdl_gl3.h>

#include <glm\glm.hpp>
#include <glm\gtc\quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/random.hpp>

#include <iostream>
#include <vector>

#include "Rigidbody.h"
#include "Solver.h"

#pragma region Forward Declarations

extern bool renderCube;

#pragma endregion

#pragma region Variables

Box* box;
SIEuler solver;

glm::vec3 gravity = glm::vec3(0.f, -9.81f,0.f);
glm::vec3 torque = glm::vec3(1.f, 1.f, 1.f);

bool isSimulation = false;
bool isCollision = true;

float box_w = 1.f;
float box_h = 1.f;
float box_d = 1.f;

glm::vec3 forces, torques;

// Times
clock_t start, diff;
float elapsedsec = 0.0f;
float sec = 15.;
bool once = false;

// For Randoms
glm::vec3 randomRotation = glm::vec3(180.f,180.f,180.);
glm::vec3 randomImpulse = glm::vec3(30.f,30.f,30.f);
glm::vec3 randomImpulseRotation = glm::vec3(10.f,10.f,10.f);

float tolerance = 0.1f;
float coef_elasticity = 0.5f;

#pragma endregion

#pragma region Functions Utils

glm::quat getRotationQuaternion(glm::vec3 axis, float angle) {
	float w = cos(angle / 2);
	glm::vec3 v = sin(angle / 2) * axis;
	return glm::normalize(glm::quat(w, v));
}

	#pragma region Randoms
glm::vec3 getRandomInitialRotation()
{
	return glm::vec3(
		glm::linearRand(-randomRotation.x, randomRotation.x),
		glm::linearRand(-randomRotation.y, randomRotation.y),
		glm::linearRand(-randomRotation.z, randomRotation.z));
}
glm::vec3 getRandomPositionInsideBox()
{
	return glm::vec3(
		glm::linearRand(-5.f + box_w, 5.f - box_w),
		glm::linearRand(0.f + box_h, 10.f - box_h),
		glm::linearRand(-5.f + box_h, 5.f - box_h));
}
glm::vec3 getRandomInitialImpulse()
{
	return glm::vec3(
		glm::linearRand(-randomImpulse.x, randomImpulse.x),
		glm::linearRand(-randomImpulse.y, randomImpulse.y),
		glm::linearRand(-randomImpulse.z, randomImpulse.z));
}
glm::vec3 getRandomTorqueAtPoint()
{
	glm::vec3 random_force = glm::vec3(
		glm::linearRand(-randomImpulse.x, randomImpulse.x),
		glm::linearRand(-randomImpulse.y, randomImpulse.y),
		glm::linearRand(-randomImpulse.z, randomImpulse.z));;

	return glm::cross(random_force, torque);
}
#pragma endregion

void InitSimulation()
{
	delete box;

	box = new Box(box_w, box_h, box_d, 1.f);

	//box->initializeState(
	//	getRandomPositionInsideBox(),
	//	getRotationQuaternion(getRandomInitialRotation(), 3.14f / 2.f),
	//	getRandomInitialImpulse(),
	//	getRandomInitialImpulse());

	box->initializeState(
		glm::vec3(0.f,5.f,0.f),
		getRotationQuaternion(glm::vec3(0.f), 3.14f / 2.f),
		glm::vec3(0.f),
		glm::vec3(0.f));

	torques = getRandomTorqueAtPoint();
}

void Timer()
{
	if (!once) // DO ONCE
	{
		// Init time
		start = clock();
		once = true;
	}
	// Timer
	diff = clock() - start;
	elapsedsec = diff / CLOCKS_PER_SEC;
	if (elapsedsec >= sec)
	{
		InitSimulation();
		elapsedsec = 0.0f;
		start = clock();
	}
}

	#pragma region Collisions

void ImpulseCorrection(RigidBody* rb, glm::vec3 contactPoint, glm::vec3 normal, float dt)
{
	glm::vec3 velL = rb->state.linearMomentum / rb->getMass();
	glm::vec3 velA = glm::inverse(rb->getInertiaTensor()) * rb->state.angularMomentum;
	
	// Response velocity
	glm::vec3 pA_der = velL + glm::cross(velA, (contactPoint - rb->getState().com));
	glm::vec3 pB_der = glm::vec3(0.f); // pongo 0 porque b es el plano
	float vRel = glm::dot(normal, (pA_der - pB_der));
	
	// Impulse
	if (vRel < 0)
	{
		float j = (-(1 + coef_elasticity) * vRel) /
			((1.f / rb->getMass()) + glm::dot(normal, (rb->getInertiaTensor() * glm::cross(glm::cross(contactPoint, normal), contactPoint))));

		/*glm::vec3 j  = (-(1 + coef_elasticity) * vRel) /
			((1.f / rb->getMass()) + normal * (rb->getInertiaTensor() * glm::cross(glm::cross(contactPoint, normal), contactPoint))));*/
		
		glm::vec3 impulse = j * normal;
		glm::vec3 impulseTorque = glm::cross((contactPoint - rb->getState().com), impulse);
		
		rb->state.linearMomentum = rb->state.linearMomentum + impulse;
		rb->state.angularMomentum = rb->state.angularMomentum + impulseTorque;
	}
}

void CollisionTime(RigidBody *rb, glm::vec3 contactPoints, glm::vec3 boxVector, glm::vec3 normal, float boxSide, float dt)
{
	float newTime = dt;
	glm::vec3 velL = rb->state.linearMomentum / rb->getMass();

	glm::vec3 tmpPosition;
	
	// Back in time
	for (int i = 0; i < 3; i++)
	{
		tmpPosition = (rb->getState().com + boxVector) + newTime * velL;	// CALCULATE NEW TMP POSITION WITH CORRECT TIME IN CORRECT POSITION
		
		if (tmpPosition.y < boxSide + tolerance)
		{
			newTime = newTime * 0.5f;
		}
		else if (tmpPosition.y > boxSide - tolerance)
		{
			newTime = newTime * 1.5f;
		}
	}

	ImpulseCorrection(rb, tmpPosition, normal, newTime);
}

void CollisionBoxWalls(RigidBody *rb, float width, float height, float depth,
	glm::vec3 boxDimensions1, glm::vec3 boxDimensions2, float dt)
{
	// 8 box vertex
	std::vector<glm::vec3> boxVertex =
	{
		glm::mat3_cast(rb->getState().rotation) * glm::vec3((width / 2.f), (height / 2.f), (depth / 2.f)) + rb->getState().com,
		glm::mat3_cast(rb->getState().rotation) * glm::vec3((width / 2.f), (-height / 2.f), (depth / 2.f)) + rb->getState().com,
		glm::mat3_cast(rb->getState().rotation) * glm::vec3((width / 2.f), (height / 2.f), (-depth / 2.f)) + rb->getState().com,
		glm::mat3_cast(rb->getState().rotation) * glm::vec3((width / 2.f), (-height / 2.f), (-depth / 2.f)) + rb->getState().com,

		glm::mat3_cast(rb->getState().rotation) * glm::vec3((-width / 2.f), (-height / 2.f), (-depth / 2.f)) + rb->getState().com,
		glm::mat3_cast(rb->getState().rotation) * glm::vec3((-width / 2.f), (-height / 2.f), (depth / 2.f)) + rb->getState().com,
		glm::mat3_cast(rb->getState().rotation) * glm::vec3((-width / 2.f), (height / 2.f), (depth / 2.f)) + rb->getState().com,
		glm::mat3_cast(rb->getState().rotation) * glm::vec3((-width / 2.f), (height / 2.f), (-depth / 2.f)) + rb->getState().com,
	};

	std::vector<glm::vec3> boxVector =
	{
		glm::mat3_cast(rb->getState().rotation) * glm::vec3((width / 2.f), (height / 2.f), (depth / 2.f)),
		glm::mat3_cast(rb->getState().rotation)* glm::vec3((width / 2.f), (-height / 2.f), (depth / 2.f)),
		glm::mat3_cast(rb->getState().rotation) *glm::vec3((width / 2.f), (height / 2.f), (-depth / 2.f)),
		glm::mat3_cast(rb->getState().rotation) *glm::vec3((width / 2.f), (-height / 2.f), (-depth / 2.f)),

		glm::mat3_cast(rb->getState().rotation) *glm::vec3((-width / 2.f), (-height / 2.f), (-depth / 2.f)),
		glm::mat3_cast(rb->getState().rotation) *glm::vec3((-width / 2.f), (-height / 2.f), (depth / 2.f)),
		glm::mat3_cast(rb->getState().rotation) *glm::vec3((-width / 2.f), (height / 2.f), (depth / 2.f)),
		glm::mat3_cast(rb->getState().rotation) *glm::vec3((-width / 2.f), (height / 2.f), (-depth / 2.f))
	};
	
	//std::vector<glm::vec3> tmpContactPoints;
	glm::vec3 n;

	for (int i = 0; i < boxVertex.size(); i++)
	{
		if (boxVertex.at(i).y <= boxDimensions1.y) // DOWN
		{
			//tmpContactPoints.push_back(boxVertex.at(i));
			n = glm::normalize(glm::vec3(0, 1, 0));
			
			rb->rollbackState();
			
			CollisionTime(rb, boxVertex.at(i), boxVector.at(i), n, boxDimensions1.y, dt);
		}
		if (boxVertex.at(i).y >= boxDimensions2.y) // UP
		{
		//	tmpContactPoints.push_back(boxVertex.at(i));
			n = glm::normalize(glm::vec3(0, -1, 0));
		}
		
		if (boxVertex.at(i).x <= boxDimensions1.x) // LEFT
		{
			//tmpContactPoints.push_back(boxVertex.at(i));
			n = glm::normalize(glm::vec3(-1, 0, 0));
		}
		if (boxVertex.at(i).x >= boxDimensions2.x) // RIGHT
		{
			//tmpContactPoints.push_back(boxVertex.at(i));
			n = glm::normalize(glm::vec3(-1, 0, 0));
		}

		if (boxVertex.at(i).z <= boxDimensions1.z) // BEHIND
		{
			//tmpContactPoints.push_back(boxVertex.at(i));
			n = glm::normalize(glm::vec3(0, 0, -1));
		}
		if (boxVertex.at(i).z >= boxDimensions2.z) // FRONT
		{
			//tmpContactPoints.push_back(boxVertex.at(i));
			n = glm::normalize(glm::vec3(0, 0, -1));
		}
	}
}

#pragma endregion

#pragma endregion

#pragma region ImGui

bool show_test_window = false;
void GUI() {
	bool show = true;
	ImGui::Begin("Physics Parameters", &show, 0);
	{	
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);//FrameRate
		
		ImGui::Checkbox("Play Simulation", &isSimulation);
		ImGui::DragFloat("Time", &sec, 0.1f);

		ImGui::DragFloat3("Gravity", glm::value_ptr(gravity), 0.1f);
		
		if (ImGui::TreeNode("Collisions"))
		{
			ImGui::Checkbox("Use Walls", &isCollision);
			
			if (isCollision)
			{
				ImGui::DragFloat("Tolerance", &tolerance, 0.1f);
				ImGui::DragFloat("Elasticity", &coef_elasticity, 0.1f);
			}

			ImGui::TreePop();
		}
	}
	
	ImGui::End();
}

#pragma endregion

#pragma region Loop Physics

void PhysicsInit() {

	srand(time(nullptr));

	InitSimulation();
	
	renderCube = true;
}

void PhysicsUpdate(float dt) {
	
	if (isSimulation)
	{
		Timer();

		forces = glm::vec3(gravity);
		torques = glm::vec3(0.f);

		solver.Update(box, forces, torques, dt);
		
		if (isCollision)
		{
			CollisionBoxWalls(box,box_w,box_h,box_h,glm::vec3(-5,0,-5), glm::vec3(5,10,5), dt);
			box->commitState();
		}
	}
	
	box->draw();
}

void PhysicsCleanup() {
	delete box;
}

#pragma endregion