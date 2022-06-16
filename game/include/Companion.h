#pragma once
#include "GameObject.h"
#include "../../libGL/include/LowRenderer/PointLight.h"

class Companion : public GameObject
{

public:
	Companion();
	Companion(std::string name, Model* model, Shader* shader,
		lm::vec3 const& position, lm::vec3 const& rotation, lm::vec3 const& scale);
	Companion(std::string name, Model* model, Shader* shader,
		lm::vec3 const& position, lm::vec3 const& rotation, lm::vec3 const& scale, Graph* parent, GLFWwindow* window);

	void			Update();

	PointLight*		GetPointLight();
};