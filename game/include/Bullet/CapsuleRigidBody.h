#pragma once
#include "RigidBody.h"

namespace Bullet
{
	class CapsuleRigidBody : public RigidBody
	{
		float _rad;
		float _height;

	public:
		CapsuleRigidBody(float rad, float height, Vec3<float> const& pos, float mass, GameObject* futurAssignedGO);

		float GetRadius() { return _rad; }
		float GetHeight() { return _height; }
	};
}