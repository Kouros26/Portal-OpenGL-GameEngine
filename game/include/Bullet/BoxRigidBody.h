#pragma once

#include "RigidBody.h"

namespace Bullet
{
	class BoxRigidBody : public RigidBody
	{
		Vec3<float> _halfextents;
		Vec3<float> _scale;

	public:
		BoxRigidBody(Vec3<float> const& halfextents, Vec3<float> const& scale,
			Vec3<float> const& pos, float mass, GameObject* futurAssignedGO);

		Vec3<float> const& GetHalfextents() { return _halfextents; }
		Vec3<float> GetScale() { return _scale; }
		void SetScale(Vec3<float>& newScale) {
			_scale = newScale;
			_rbShape->setLocalScaling(btVector3(newScale.X(), newScale.Y(), newScale.Z()));
			}
	};
}