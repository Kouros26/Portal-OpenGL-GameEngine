#include "../include/Gun.h"

Gun::Gun(Graph* player, GLFWwindow* window)
{
	this->name = "Gun";
	this->model = Singleton::resources->Get<Model>("Assets/Meshes\\portal_gun\\PortalGun.FBX");
	this->shader = Singleton::resources->Get<Shader>("LitShader");
	this->pos = lm::vec3(0);
	this->rot = lm::vec3(0,-90,90);
	this->scale = lm::vec3(1);
	this->parent = nullptr;
	this->localTransform = lm::mat4::createTransformMatrix(this->pos, this->rot, this->scale);
	this->globalTransform = this->localTransform;
}

void Gun::Update()
{

}
