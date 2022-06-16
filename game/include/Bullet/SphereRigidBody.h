#pragma once
#include "RigidBody.h"

namespace Bullet
{
	class SphereRigidBody : public RigidBody
	{
		float _rad;

	public:
		SphereRigidBody(float rad, Vec3<float> const& pos, float mass, GameObject* futurAssignedGO);

		float GetRadius() { return _rad; }
	};
}