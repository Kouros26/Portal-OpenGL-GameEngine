#pragma once
#include "../Ressource/ResourcesManager.h"
#include "../Ressource/AssimpShader.h"
#include "../../../game/include/GameObject.h"

class Light : public Component
{
protected:
	lm::vec4 diffuseColor;
	lm::vec4 ambiantColor;
	lm::vec4 specularColor;
	float constant;
	float linear;
	float quadratic;

public:
	virtual void UpdateLights(Shader* shader, int index) = 0;
	lm::vec4& GetDiffuseColor() { return diffuseColor; }
	lm::vec4& GetAmbiantColor() { return ambiantColor; }
	lm::vec4& GetSpecularColor() { return specularColor; }
	float& GetConstant() { return constant; }
	float& GetLinear() { return linear; }
	float& GetQuadratic() { return quadratic; }
};