#include "..\include\GameObject.h"
#include "../include/Bullet/BoxRigidBody.h"
#include "../include/Bullet/CapsuleRigidBody.h"
#include "../include/Bullet/SphereRigidBody.h"
#include "../../libGL/include/LowRenderer/PointLight.h"
#include "../../libGL/include/LowRenderer/SpotLight.h"
#include "../../libGL/include/Application.h"
#include "../include/Music.h"

GameObject::GameObject()
{
}

GameObject::GameObject(std::string name, Model* model, Shader* shader, lm::vec3 const& position, lm::vec3 const& rotation, lm::vec3 const& scale)
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

GameObject::GameObject(std::string name, Model* model, Shader* shader, lm::vec3 const& position, lm::vec3 const& rotation, lm::vec3 const& scale, Graph* parent, GLFWwindow* window)
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

void GameObject::UpdateMatrices()
{
	RigidBody* rb = (RigidBody*)this->GetRigidBody();

	if (this->parent != nullptr && rb != nullptr)
	{
		this->globalTransform = this->parent->GetGlobalMatrix() * this->localTransform;
		Mat4 glMat4 = this->parent->GetGlobalMatrix() * this->localTransform;
	/*	if (Singleton::cam->GetFrontInput() != 0 || Singleton::cam->GetRightInput() != 0 || Singleton::cam->GetUpInput() != 0)
		{
			this->globalTransform = this->localTransform;
		}*/
		vec3 glPos = vec3(glMat4[3][0], glMat4[3][1], glMat4[3][2]);
		vec3 glRot = glMat4.extractRotation(glMat4.extractScale());
		RepositionRigidBody(glPos, glRot);
	}
	else if (this->parent != nullptr)
		this->globalTransform = this->parent->GetGlobalMatrix() * this->localTransform;

	this->pos.X() = this->globalTransform[3][0];
	this->pos.Y() = this->globalTransform[3][1];
	this->pos.Z() = this->globalTransform[3][2];

	this->scale = this->globalTransform.extractScale();

	this->rot = this->globalTransform.extractRotation(this->scale);

	if (this->model != nullptr)
		this->model->Draw(this->globalTransform, *this->shader);

	for (int i = 0; i < _components.size(); i++)
	{
		if (dynamic_cast<Music*>(_components[i]) != nullptr)
			dynamic_cast<Music*>(_components[i])->Update(this->pos);
	}

	for (int i = 0; i < this->childs.size(); i++)
	{
		this->childs[i]->UpdateMatrices();
	}
}

void GameObject::RepositionRigidBody(vec3 const& newPosition, vec3 const& newOrientation)
{
	RigidBody* rb = (RigidBody*)GetRigidBody();
	if (rb == nullptr) return;

	Bullet::CustomMotionState* cust = rb->GetMS();

	btVector3 position(newPosition.X(), newPosition.Y(), newPosition.Z());
	btQuaternion orientation(newOrientation.X(), newOrientation.Y(), newOrientation.Z());

	btTransform initialTransform;

	initialTransform.setOrigin(position);
	initialTransform.setRotation(orientation);

	rb->GetRB()->setWorldTransform(initialTransform);
	cust->setWorldTransform(initialTransform);
}

void GameObject::OnCollisionEnter(GameObject* me, GameObject* other, btPersistentManifold const* manifold)
{
	printf("%s collision with %s \n", me->GetName().c_str(), other->GetName().c_str());
}

void GameObject::OnCollisionExit(GameObject* me, GameObject* other, btPersistentManifold const* manifold)
{
	printf("%s exit collision with %s \n", me->GetName().c_str(), other->GetName().c_str());
}

void GameObject::CreateBoxRigidBody(lm::vec3 const& halfextents, lm::vec3 const& scale, float mass)
{
	BoxRigidBody* rb = new BoxRigidBody(halfextents, scale,
		this->pos, mass, this);
	rb->GetMS()->SetGraphicMatrix(&this->GetGlobalMatrix());
	rb->GetShapeType() = RigidBodyType::BOX;
	_components.push_back(rb);
	WorldPhysics::GetInstance().GetGameObjects().push_back(this);
}

void GameObject::CreateCapsuleRigidBody(float height, float rad, float mass)
{

	CapsuleRigidBody* rb = new CapsuleRigidBody(rad, height, this->pos, mass, this);
	rb->GetMS()->SetGraphicMatrix(&this->GetGlobalMatrix());
	rb->GetShapeType() = RigidBodyType::CAPSULE;
	_components.push_back(rb);
	WorldPhysics::GetInstance().GetGameObjects().push_back(this);
}

void GameObject::CreateSphereRigidBody(float rad, float mass)
{
	SphereRigidBody* rb = new SphereRigidBody(rad, this->pos, mass, this);
	rb->GetMS()->SetGraphicMatrix(&this->GetGlobalMatrix());
	rb->GetShapeType() = RigidBodyType::SPHERE;
	_components.push_back(rb);
	WorldPhysics::GetInstance().GetGameObjects().push_back(this);
}

void GameObject::CreatePointLight(lm::vec3 const& ambient, lm::vec3 const& diffuse,
	lm::vec3 const& specular, float constant, float linear, float quadratic)
{
	PointLight* light = new PointLight(ambient, diffuse, specular, &this->pos, constant, linear, quadratic);
	Application* app = (Application*)glfwGetWindowUserPointer(Singleton::window);
	app->getPointLights().push_back(light);
	_components.push_back(light);
}

void GameObject::CreateSpotLight(lm::vec3 const& ambient, lm::vec3 const& diffuse,
	lm::vec3 const& specular, lm::vec3& direction, float cutOff, float outerCutOff, float constant, float linear, float quadratic)
{
	SpotLight* light = new SpotLight(ambient, diffuse, specular, this->pos, direction,
		cutOff, outerCutOff, constant, linear, quadratic);
	Application* app = (Application*)glfwGetWindowUserPointer(Singleton::window);
	app->getSpotLights().push_back(light);
	_components.push_back(light);
}

void GameObject::CreateSoundSource(irrklang::ISound* sound, float minRange, float maxRange)
{
	Music* music = new Music(sound, minRange, maxRange);
	_components.push_back(music);
}

void* GameObject::GetRigidBody()
{
	for (int i = 0; i < this->GetComponents().size(); i++)
	{
		if(dynamic_cast<RigidBody*>(this->GetComponents()[i]) != nullptr)
			return dynamic_cast<RigidBody*>(this->GetComponents()[i]);
	}

	return nullptr;
}

void GameObject::SaveObject(rapidjson::PrettyWriter<rapidjson::StringBuffer>& buffer)
{
	buffer.StartObject();

	buffer.Key("ObjectName");
	buffer.String(this->name.c_str());

	buffer.Key("ShaderName");
	if (this->shader != nullptr)
	{
		buffer.String(this->shader->name.c_str());

		buffer.Key("VertexShaderPath");
		buffer.String(this->shader->vertexPath.c_str());

		buffer.Key("FragmentShaderPath");
		buffer.String(this->shader->fragmentPath.c_str());

		buffer.Key("GeometryShaderPath");
		buffer.String(this->shader->geometryPath.c_str());
	}
	else
		buffer.String("Null");

	buffer.Key("ModelPath");
	if (this->model != nullptr)
		buffer.String(this->model->path.c_str());
	else
		buffer.String("Null");

	buffer.Key("Position");
	buffer.SetFormatOptions(rapidjson::PrettyFormatOptions::kFormatSingleLineArray);
	buffer.StartArray();
	buffer.Double(this->pos.X());
	buffer.Double(this->pos.Y());
	buffer.Double(this->pos.Z());
	buffer.EndArray();
	buffer.SetFormatOptions(rapidjson::PrettyFormatOptions::kFormatDefault);

	buffer.Key("Rotation");
	buffer.SetFormatOptions(rapidjson::PrettyFormatOptions::kFormatSingleLineArray);
	buffer.StartArray();
	buffer.Double(this->rot.X());
	buffer.Double(this->rot.Y());
	buffer.Double(this->rot.Z());
	buffer.EndArray();
	buffer.SetFormatOptions(rapidjson::PrettyFormatOptions::kFormatDefault);

	buffer.Key("Scale");
	buffer.SetFormatOptions(rapidjson::PrettyFormatOptions::kFormatSingleLineArray);
	buffer.StartArray();
	buffer.Double(this->scale.X());
	buffer.Double(this->scale.Y());
	buffer.Double(this->scale.Z());
	buffer.EndArray();
	buffer.SetFormatOptions(rapidjson::PrettyFormatOptions::kFormatDefault);

	buffer.Key("Component");
	buffer.StartArray();
	for (int i = 0; i < this->_components.size(); i++)
	{
		buffer.StartObject();
		buffer.Key("Type");
		std::string typeName = this->_components[i]->GetComponentName();
		buffer.String(typeName.c_str());

		if (typeName == "PointLight" || typeName == "SpotLight")
		{
			buffer.Key("AmbiantColor");
			buffer.SetFormatOptions(rapidjson::PrettyFormatOptions::kFormatSingleLineArray);
			buffer.StartArray();
			buffer.Double(dynamic_cast<Light*>(this->_components[i])->GetAmbiantColor().X());
			buffer.Double(dynamic_cast<Light*>(this->_components[i])->GetAmbiantColor().Y());
			buffer.Double(dynamic_cast<Light*>(this->_components[i])->GetAmbiantColor().Z());
			buffer.EndArray();
			buffer.SetFormatOptions(rapidjson::PrettyFormatOptions::kFormatDefault);

			buffer.Key("DiffuseColor");
			buffer.SetFormatOptions(rapidjson::PrettyFormatOptions::kFormatSingleLineArray);
			buffer.StartArray();
			buffer.Double(dynamic_cast<Light*>(this->_components[i])->GetDiffuseColor().X());
			buffer.Double(dynamic_cast<Light*>(this->_components[i])->GetDiffuseColor().Y());
			buffer.Double(dynamic_cast<Light*>(this->_components[i])->GetDiffuseColor().Z());
			buffer.EndArray();
			buffer.SetFormatOptions(rapidjson::PrettyFormatOptions::kFormatDefault);

			buffer.Key("SpecularColor");
			buffer.SetFormatOptions(rapidjson::PrettyFormatOptions::kFormatSingleLineArray);
			buffer.StartArray();
			buffer.Double(dynamic_cast<Light*>(this->_components[i])->GetSpecularColor().X());
			buffer.Double(dynamic_cast<Light*>(this->_components[i])->GetSpecularColor().Y());
			buffer.Double(dynamic_cast<Light*>(this->_components[i])->GetSpecularColor().Z());
			buffer.EndArray();
			buffer.SetFormatOptions(rapidjson::PrettyFormatOptions::kFormatDefault);

			buffer.Key("Constant");
			buffer.Double(dynamic_cast<Light*>(this->_components[i])->GetConstant());

			buffer.Key("Linear");
			buffer.Double(dynamic_cast<Light*>(this->_components[i])->GetLinear());

			buffer.Key("Quadratic");
			buffer.Double(dynamic_cast<Light*>(this->_components[i])->GetQuadratic());

			if (typeName == "SpotLight")
			{
				buffer.Key("Direction");
				buffer.SetFormatOptions(rapidjson::PrettyFormatOptions::kFormatSingleLineArray);
				buffer.StartArray();
				buffer.Double(dynamic_cast<SpotLight*>(this->_components[i])->GetDirection().X());
				buffer.Double(dynamic_cast<SpotLight*>(this->_components[i])->GetDirection().Y());
				buffer.Double(dynamic_cast<SpotLight*>(this->_components[i])->GetDirection().Z());
				buffer.EndArray();
				buffer.SetFormatOptions(rapidjson::PrettyFormatOptions::kFormatDefault);

				buffer.Key("CutOff");
				buffer.Double(dynamic_cast<SpotLight*>(this->_components[i])->GetCutOff());

				buffer.Key("OuterCutOff");
				buffer.Double(dynamic_cast<SpotLight*>(this->_components[i])->GetOuterCutOff());
			}
		}

		else if (typeName == "BoxRigidBody" || typeName == "CapsuleRigidBody" || typeName == "SphereRigidBody")
		{
			buffer.Key("Mass");
			buffer.Double(dynamic_cast<RigidBody*>(this->_components[i])->GetMass());

			if (typeName == "BoxRigidBody")
			{
				buffer.Key("HalfExtents");
				buffer.SetFormatOptions(rapidjson::PrettyFormatOptions::kFormatSingleLineArray);
				buffer.StartArray();
				buffer.Double(dynamic_cast<BoxRigidBody*>(this->_components[i])->GetHalfextents().X());
				buffer.Double(dynamic_cast<BoxRigidBody*>(this->_components[i])->GetHalfextents().Y());
				buffer.Double(dynamic_cast<BoxRigidBody*>(this->_components[i])->GetHalfextents().Z());
				buffer.EndArray();
				buffer.SetFormatOptions(rapidjson::PrettyFormatOptions::kFormatDefault);

				buffer.Key("Scale");
				buffer.SetFormatOptions(rapidjson::PrettyFormatOptions::kFormatSingleLineArray);
				buffer.StartArray();
				buffer.Double(dynamic_cast<BoxRigidBody*>(this->_components[i])->GetScale().X());
				buffer.Double(dynamic_cast<BoxRigidBody*>(this->_components[i])->GetScale().Y());
				buffer.Double(dynamic_cast<BoxRigidBody*>(this->_components[i])->GetScale().Z());
				buffer.EndArray();
				buffer.SetFormatOptions(rapidjson::PrettyFormatOptions::kFormatDefault);
			}

			else if (typeName == "CapsuleRigidBody")
			{
				buffer.Key("Rad");
				buffer.Double(dynamic_cast<CapsuleRigidBody*>(this->_components[i])->GetRadius());

				buffer.Key("Height");
				buffer.Double(dynamic_cast<CapsuleRigidBody*>(this->_components[i])->GetHeight());
			}

			else if (typeName == "SphereRigidBody")
			{
				buffer.Key("Rad");
				buffer.Double(dynamic_cast<SphereRigidBody*>(this->_components[i])->GetRadius());
			}
		}

		buffer.EndObject();
	}
	buffer.EndArray();

	buffer.Key("Childs");
	buffer.StartArray();
	for (int i = 0; i < this->childs.size(); i++)
	{
		this->childs[i]->SaveObject(buffer);
	}
	buffer.EndArray();
	buffer.EndObject();
}