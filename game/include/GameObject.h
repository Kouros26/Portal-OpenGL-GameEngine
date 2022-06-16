#pragma once
#include "../../libGL/include/Core/DataStructure/Graph.h"
#include "../../game/include/Component.h"
#include "btBulletCollisionCommon.h"
#include "btBulletDynamicsCommon.h"
#include "../IrrKlang/include/irrKlang.h"

using namespace lm;

class btRigidBody;

class GameObject : public Graph
{
protected:
	std::vector<Component*> _components;

public:
	GameObject();
	GameObject(std::string name, Model* model, Shader* shader, lm::vec3 const& position, lm::vec3 const& rotation, lm::vec3 const& scale);
	GameObject(std::string name, Model* model, Shader* shader, lm::vec3 const& position, lm::vec3 const& rotation, lm::vec3 const& scale, Graph* parent, GLFWwindow* window);
	~GameObject() = default;

	void				UpdateMatrices() override;
	void				RepositionRigidBody(vec3 const& newPosition, vec3 const& newOrientation);
	virtual void		OnCollisionEnter(GameObject* me, GameObject* other, btPersistentManifold const* manifold);
	virtual void		OnCollisionExit(GameObject* me, GameObject* other, btPersistentManifold const* manifold);
	void				CreateBoxRigidBody(lm::vec3 const& halfextents, lm::vec3 const& scale,
							float mass);
	void				CreateCapsuleRigidBody(float height, float rad,
							float mass);
	void				CreateSphereRigidBody(float rad, float mass);
	void				CreatePointLight(lm::vec3 const& ambient,
							lm::vec3 const& diffuse, lm::vec3 const& specular,
							float constant, float linear, float quadratic);
	void				CreateSpotLight(lm::vec3 const& ambient,
							lm::vec3 const& diffuse, lm::vec3 const& specular,
							lm::vec3& direction, float cutOff,
							float outerCutOff, float constant, float linear, float quadratic);

	void				CreateSoundSource(irrklang::ISound* sound, float minRange, float maxRange);

	void*					GetRigidBody();
	std::vector<Component*>&	GetComponents() { return _components; }

	void SaveObject(rapidjson::PrettyWriter<rapidjson::StringBuffer>& buffer);
};