#pragma once
#include "Bullet/RigidBody.h"
#include "Singleton.h"

using namespace Bullet;

class Platform : public GameObject
{
	float x = 0.f;
	int amplitude;
public:

	Platform();
	Platform(std::string name, Model* model, Shader* shader, lm::vec3 const& position, lm::vec3 const& rotation, lm::vec3 const& scale);
	Platform(std::string name, Model* model, Shader* shader, lm::vec3 const& position, lm::vec3 const& rotation, lm::vec3 const& scale, Graph* parent, GLFWwindow* window);

	void Update(float deltaTime);
	void RepositionRigidBody(vec3 const& newPosition, vec3 const& newOrientation);
};