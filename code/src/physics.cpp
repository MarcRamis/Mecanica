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

glm::vec3 gravity = glm::vec3(0.f, 9.f,0.f);
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
glm::vec3 randomForceTorque = glm::vec3(10.f,10.f,10.f);
glm::vec3 randomImpulseRotation = glm::vec3(10.f,10.f,10.f);

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

void ResetSimulation()
{
	delete box;

	box = new Box(box_w, box_h, box_d, 1.f);

	box->initializeState(
		getRandomPositionInsideBox(),
		getRotationQuaternion(getRandomInitialRotation(), 3.14f / 2.f),
		getRandomInitialImpulse(),
		getRandomInitialImpulse());

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
		ResetSimulation();
		elapsedsec = 0.0f;
		start = clock();
	}
}

	#pragma region Collisions

void CollisionBoxWalls(glm::vec3& com, glm::quat& rot, float width, float height, float depth,
	glm::vec3 boxDimensions1, glm::vec3 boxDimensions2)
{
	// 6 WALLS - 6 FACES
	// 6 FACES OF THE BOX
	std::vector<float> boxFaces =
	{ com.x + width / 2, com.x - width / 2,
	com.y + height / 2, com.y - height / 2,
	com.z + depth / 2, com.z - depth / 2 };

	std::vector<float> wallFaces
	{
		boxDimensions1.x, boxDimensions1.y, boxDimensions1.z,
		boxDimensions2.x, boxDimensions2.y, boxDimensions2.z
	};

	// BUT I DONT WANT FACES I WANT VERTEXS
	std::vector<glm::vec3> boxVertex =
	{
		glm::mat3_cast(rot) * glm::vec3((width / 2.f), (height / 2.f), (depth / 2.f)) + com,
		glm::mat3_cast(rot) * glm::vec3((width / 2.f), (-height / 2.f), (depth / 2.f)) + com,
		glm::mat3_cast(rot) * glm::vec3((width / 2.f), (height / 2.f), (-depth / 2.f)) + com,
		glm::mat3_cast(rot) * glm::vec3((width / 2.f), (-height / 2.f), (-depth / 2.f)) + com,

		glm::mat3_cast(rot) * glm::vec3((-width / 2.f), (-height / 2.f), (-depth / 2.f)) + com,
		glm::mat3_cast(rot) * glm::vec3((-width / 2.f), (-height / 2.f), (depth / 2.f)) + com,
		glm::mat3_cast(rot) * glm::vec3((-width / 2.f), (height / 2.f), (depth / 2.f)) + com,
		glm::mat3_cast(rot) * glm::vec3((-width / 2.f), (height / 2.f), (-depth / 2.f)) + com,
	};

	std::vector<glm::vec3> tmp;

	for (int i = 0; i < boxVertex.size(); i++)
	{
		if (boxVertex.at(i).y < boxDimensions1.y)
		{
			std::cout << i << "collision down" << std::endl;
			tmp.push_back(boxVertex.at(i));
			// REBOTE
		}
		if (boxVertex.at(i).y > boxDimensions2.y)
		{
			std::cout << i << "collision up" << std::endl;
			tmp.push_back(boxVertex.at(i));
		}
		
		if (boxVertex.at(i).x < boxDimensions2.x)
		{
			std::cout << i << "collision left" << std::endl;
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
		ImGui::DragFloat("Timer", &sec, 0.1f);

		if (ImGui::TreeNode("Force"))
		{
			ImGui::DragFloat3("Gravity", glm::value_ptr(gravity), 0.1f);

			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Randoms"))
		{
			ImGui::DragFloat3("Rotation", glm::value_ptr(randomRotation), 0.1f);
			ImGui::DragFloat3("Impulse", glm::value_ptr(randomImpulse), 0.1f);

			ImGui::TreePop();
		}
		
		if (ImGui::TreeNode("Collisions"))
		{
			ImGui::Checkbox("Play Simulation", &isCollision);
			
			if (isCollision)
			{

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

	box = new Box(box_w,box_h,box_d,1.f);
	
	box->initializeState(
		getRandomPositionInsideBox(),
		getRotationQuaternion(getRandomInitialRotation(), 3.14f / 2.f),
		getRandomInitialImpulse(),
		getRandomInitialImpulse());
	
	renderCube = true;

	//torques = glm::cross(forceTorque, torque);
	torques = getRandomTorqueAtPoint();
}

void PhysicsUpdate(float dt) {
	
	forces = glm::vec3(gravity);
	//forces = glm::vec3(0.f);

	torques = glm::vec3(0.f);

	if (isSimulation)
	{
		Timer();

		solver.Update(box, forces, torques, dt);

		if (isCollision)
		{
			CollisionBoxWalls(box->getState().com, box->getState().rotation,box_w,box_h,box_h, // box properties
				glm::vec3(-5,0,-5), glm::vec3(5,10,5)); // wall properites
		}
	}
	
	box->draw();
}

void PhysicsCleanup() {
	delete box;
}

#pragma endregion