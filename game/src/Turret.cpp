#include "../include/Turret.h"

Turret::Turret(Model* model, std::string shaderPath)
{
	this->name = "Turret";
	this->model = model;
	this->shader = Singleton::resources->Get<Shader>(shaderPath);
	this->pos = lm::vec3(20, 0, -10);
	this->rot = lm::vec3(0, 0, 0);
	this->scale = lm::vec3(1, 1, 1);
	this->parent = nullptr;
	this->localTransform = lm::mat4::createTransformMatrix(pos, rot, scale);
	this->globalTransform = this->localTransform;
}

void Turret::Update()
{
	this->globalTransform = lm::mat4::lookAtRotation(pos, Singleton::cam->GetPosition(), vec3::up).getInverse();
}
