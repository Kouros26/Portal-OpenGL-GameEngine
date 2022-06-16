#include "../../include/LowRenderer/SpotLight.h"

SpotLight::SpotLight(lm::vec3 const& ambient, lm::vec3 const& diffuse, lm::vec3 const& specular, lm::vec3 const& position, lm::vec3& direction, float cutOff, float outerCutOff, float constant, float linear, float quadratic)
{
	this->name = "SpotLight";
	this->ambiantColor = ambient;
	this->diffuseColor = diffuse;
	this->specularColor = specular;
	this->position = position;
	this->direction = direction;
	this->cutOff = cutOff;
	this->outerCutOff = outerCutOff;
	this->constant = constant;
	this->linear = linear;
	this->quadratic = quadratic;
}

void SpotLight::UpdateLights(Shader* shader, int index)
{
	std::string str = "spotLight[" + std::to_string(index) + "].";
	glUseProgram(shader->GetShaderProgram());
	glUniform3fv(glGetUniformLocation(shader->GetShaderProgram(), (str + std::string("position")).c_str()), 1, &this->position[0]);
	glUniform3fv(glGetUniformLocation(shader->GetShaderProgram(), (str + std::string("direction")).c_str()), 1, &this->direction[0]);
	glUniform4fv(glGetUniformLocation(shader->GetShaderProgram(), (str + std::string("ambient")).c_str()), 1, &this->ambiantColor[0]);
	glUniform4fv(glGetUniformLocation(shader->GetShaderProgram(), (str + std::string("diffuse")).c_str()), 1, &this->diffuseColor[0]);
	glUniform4fv(glGetUniformLocation(shader->GetShaderProgram(), (str + std::string("specular")).c_str()), 1, &this->specularColor[0]);
	glUniform1f(glGetUniformLocation(shader->GetShaderProgram(), (str + std::string("constant")).c_str()), this->constant);
	glUniform1f(glGetUniformLocation(shader->GetShaderProgram(), (str + std::string("linear")).c_str()), this->linear);
	glUniform1f(glGetUniformLocation(shader->GetShaderProgram(), (str + std::string("quadratic")).c_str()), this->quadratic);
	glUniform1f(glGetUniformLocation(shader->GetShaderProgram(), (str + std::string("cutOff")).c_str()), this->cutOff);
	glUniform1f(glGetUniformLocation(shader->GetShaderProgram(), (str + std::string("outerCutOff")).c_str()), this->outerCutOff);
}

void SpotLight::SetPositionDirection(Camera* cam)
{
	this->position = cam->GetPosition();
	this->direction = cam->GetFoward();
}
