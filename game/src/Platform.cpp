#include "../include/Platform.h"

using namespace Bullet;

Platform::Platform() : amplitude(5)
{
	this->model = Singleton::resources->Get<Model>("Assets/Meshes\\Ultimate_Modular_Sci-Fi\\OBJ\\FloorTile_Empty.obj");
	this->name = "Platform";
	this->shader = Singleton::resources->Get<Shader>("LitShader");
	this->pos = lm::vec3(0, 2, 0);
	this->rot = lm::vec3(0, 0, 0);
	this->scale = lm::vec3(1, 1, 1);
	this->parent = nullptr;
	this->localTransform = lm::mat4::createTransformMatrix(pos, rot, scale);
	this->globalTransform = this->localTransform;
	this->CreateBoxRigidBody(lm::vec3(1.f,0.1f,1.f), lm::vec3(1.f,0.3f,1.f), 0);
	Singleton::sceneGraph.push_back(this);

	RigidBody* rb = (RigidBody*)GetRigidBody();
	if (rb == nullptr) return;
	rb->SetRBState(rbState::STATIC);
}

Platform::Platform(std::string name, Model* model, Shader* shader, lm::vec3 const& position, lm::vec3 const& rotation, lm::vec3 const& scale) : amplitude(5)
{
	/* GameObject */
	this->name = name;
	this->model = model;
	this->shader = shader;
	this->parent = parent;
	this->parent = nullptr;
	this->localTransform = lm::mat4::createTransformMatrix(position, rotation, scale);
	this->globalTransform = this->localTransform;
	this->pos = position;
	this->rot = rotation;
	this->scale = scale;
}

Platform::Platform(std::string name, Model* model, Shader* shader, lm::vec3 const& position, lm::vec3 const& rotation, lm::vec3 const& scale, Graph* parent, GLFWwindow* window) : amplitude(5)
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

void Platform::Update(float deltaTime)
{
	RepositionRigidBody(vec3(pos.X() + sinf(x) * amplitude * deltaTime, pos.Y(), pos.Z()), vec3(0.f));

	x += 0.005f;
}

void Platform::RepositionRigidBody(vec3 const& newPosition, vec3 const& newOrientation)
{

	RigidBody* rb = (RigidBody*)GetRigidBody();
	if (rb == nullptr) return;

	Bullet::CustomMotionState* cust = rb->GetMS();

	btVector3 position(newPosition.X(), newPosition.Y(), newPosition.Z());
	btQuaternion orientation(0, newOrientation.Y(), 0);

	btTransform initialTransform;

	initialTransform.setOrigin(position);
	initialTransform.setRotation(orientation);

	rb->GetRB()->setWorldTransform(initialTransform);
	cust->setWorldTransform(initialTransform);
}
