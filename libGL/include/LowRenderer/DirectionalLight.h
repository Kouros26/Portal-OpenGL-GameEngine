#pragma once
#include "Light.h"

class Shader;

class DirectionalLight : public Light
{
	lm::vec3 direction;

public:

	DirectionalLight(lm::vec3& ambient, lm::vec3& diffuse, lm::vec3& specular, lm::vec3& direction);
	void UpdateLights(Shader* shader, int index) override;
};