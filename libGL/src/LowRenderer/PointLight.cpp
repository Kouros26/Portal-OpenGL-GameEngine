#include "../../include/LowRenderer/PointLight.h"

PointLight::PointLight(lm::vec3 const& ambient, lm::vec3 const& diffuse, lm::vec3 const& specular, lm::vec3* pos, float constant, float linear, float quadratic) : position(*pos)
{
	this->name = "PointLight";
	this->ambiantColor = ambient;
	this->diffuseColor = diffuse;
	this->specularColor = specular;
	this->constant = constant;
	this->linear = linear;
	this->quadratic = quadratic;
}

void PointLight::UpdateLights(Shader* shader, int index)
{
	std::string str = "pointLights[" + std::to_string(index) + "].";
	glUseProgram(shader->GetShaderProgram());
	glUniform3fv(glGetUniformLocation(shader->GetShaderProgram(), (str + std::string("position")).c_str()), 1, &this->position[0]);
	glUniform4fv(glGetUniformLocation(shader->GetShaderProgram(), (str + std::string("ambient")).c_str()), 1, &this->ambiantColor[0]);
	glUniform4fv(glGetUniformLocation(shader->GetShaderProgram(), (str + std::string("diffuse")).c_str()), 1, &this->diffuseColor[0]);
	glUniform4fv(glGetUniformLocation(shader->GetShaderProgram(), (str + std::string("specular")).c_str()), 1, &this->specularColor[0]);
	glUniform1f(glGetUniformLocation(shader->GetShaderProgram(), (str + std::string("constant")).c_str()), this->constant);
	glUniform1f(glGetUniformLocation(shader->GetShaderProgram(), (str + std::string("linear")).c_str()), this->linear);
	glUniform1f(glGetUniformLocation(shader->GetShaderProgram(), (str + std::string("quadratic")).c_str()), this->quadratic);
}
