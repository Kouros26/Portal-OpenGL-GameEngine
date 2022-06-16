#include "..\include\Companion.h"
#include "../include/Singleton.h"
#include "../include/Bullet/RigidBody.h"

Companion::Companion()
{
	this->model = Singleton::resources->Get<Model>("Assets/Meshes\\companion_cube\\scene.gltf");
	this->name = "Companion";
	this->shader = Singleton::resources->Get<Shader>("LitShader");
	this->pos = lm::vec3(0, 0, 6);
	this->rot = lm::vec3(0, 0, 0);
	this->scale = lm::vec3(1, 1, 1);
	this->parent = nullptr;
	this->localTransform = lm::mat4::createTransformMatrix(pos, rot, scale);
	this->globalTransform = this->localTransform;
	this->CreateBoxRigidBody(lm::vec3(0.5f, 0.5f, 0.5f), lm::vec3(1.f, 1.f, 1.f), 10);
	this->CreatePointLight(lm::vec3(1.0f, 1.0f, 1.0f), lm::vec3(0.8f, 0.8f, 0.8f), lm::vec3(1.0f, 1.0f, 1.0f), 1.0f, 0.09f, 0.032f);
	RigidBody* rb = (RigidBody*)this->GetRigidBody();
	rb->GetRB()->setActivationState(DISABLE_DEACTIVATION);
	Singleton::sceneGraph.push_back(this);
}

Companion::Companion(std::string name, Model* model, Shader* shader, lm::vec3 const& position, lm::vec3 const& rotation, lm::vec3 const& scale)
{
	this->name = name;
	this->model = model;
	this->shader = shader;
	this->parent = nullptr;
	this->localTransform = lm::mat4::createTransformMatrix(position, rotation, scale);
	this->globalTransform = this->localTransform;
	this->pos = position;
	this->rot = rotation;
	this->scale = scale;
}


Companion::Companion(std::string name, Model* model, Shader* shader, lm::vec3 const& position, lm::vec3 const& rotation, lm::vec3 const& scale, Graph* parent, GLFWwindow* window)
{
	this->name = name;
	this->model = model;
	this->shader = shader;
	this->parent = parent;
	this->parent->AddChild(this, window);
	this->localTransform = lm::mat4::createTransformMatrix(position, rotation, scale);
	this->globalTransform = this->parent->GetGlobalMatrix() * this->localTransform;
	this->pos = position;
	this->rot = rotation;
	this->scale = scale;
}

void Companion::Update()
{
}

PointLight* Companion::GetPointLight()
{
	for (int i = 0; i < this->GetComponents().size(); i++)
	{
		if (dynamic_cast<PointLight*>(this->GetComponents()[i]) != nullptr)
			return dynamic_cast<PointLight*>(this->GetComponents()[i]);
	}

	return nullptr;
}
