#include "../../include/LowRenderer/DirectionalLight.h"
#include "../../include/Core/Debug/Log.h"

DirectionalLight::DirectionalLight(lm::vec3& ambient, lm::vec3& diffuse, lm::vec3& specular, lm::vec3& direction)
{
	this->name = "DirectionalLight";
	this->direction = direction;
	this->ambiantColor = ambient;
	this->diffuseColor = diffuse;
	this->specularColor = specular;
}

void DirectionalLight::UpdateLights(Shader* shader, int index)
{
	std::string str = "dirLight[" + std::to_string(index) + "].";
	glUseProgram(shader->GetShaderProgram());
	glUniform3fv(glGetUniformLocation(shader->GetShaderProgram(), (str + std::string("direction")).c_str()), 1, &this->direction[0]);
	glUniform4fv(glGetUniformLocation(shader->GetShaderProgram(), (str + std::string("ambient")).c_str()), 1, &this->ambiantColor[0]);
	glUniform4fv(glGetUniformLocation(shader->GetShaderProgram(), (str + std::string("diffuse")).c_str()), 1, &this->diffuseColor[0]);
	glUniform4fv(glGetUniformLocation(shader->GetShaderProgram(), (str + std::string("specular")).c_str()), 1, &this->specularColor[0]);
}
