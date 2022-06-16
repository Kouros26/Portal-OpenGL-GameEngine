#pragma once
#include "Vec3/Vec3.h"
#include "GameObject.h"
#include "../../../libGL/include/LowRenderer/Camera.h"
#include "Companion.h"

using namespace lm;


class Player : public GameObject
{
	float _raycastRange = 100.f;
	float _RangeToGrabCompanion = 5.f;
	bool _holdCompanion = false;
	Companion* _lastCompanionHold = nullptr;
	vec3 _childCompanionPos;

public:
	struct RayResult
	{
		GameObject* _gameObject = nullptr;
		btRigidBody* _rb = nullptr;
		lm::vec3 _hitPoint;
	};

	Player();
	Player(std::string name, Model* model, Shader* shader, lm::vec3 position, lm::vec3 rotation, lm::vec3 scale);
	Player(std::string name, Model* model, Shader* shader, lm::vec3 position, lm::vec3 rotation, lm::vec3 scale,
		Graph* parent, GLFWwindow* window);

	float&				GetRaycastRange() { return _raycastRange; }

	void				OnCollisionEnter(GameObject* me, GameObject* other, btPersistentManifold const* manifold) override;
	void				OnCollisionExit(GameObject* me, GameObject* other, btPersistentManifold const* manifold) override;
	void				Update(float deltaTime);
	void				Move(float deltaTime);
	static lm::vec3		GetPickingRay(int x, int y);
	static bool			Raycast(lm::vec3 const& startPos, lm::vec3 const& direction, RayResult& output);
	void				RotationOnY();
	bool				InRangeOfCompanionCube(RayResult& result);
	void				GrabbCompanionCube(RayResult& result);
	void				UnGrabbCompanionCube(RayResult& result);
	void				UpdateCompanion();

	bool				GetHoldCC() { return _holdCompanion; }
};