#pragma once

#include "btBulletCollisionCommon.h"
#include "Mat4/Mat4.h"

using namespace lm;

namespace Bullet
{
	class CustomMotionState : public btDefaultMotionState
	{
		mat4* GraphicMatrix = nullptr;

	public:
		CustomMotionState(const btTransform& transform) : btDefaultMotionState(transform) {}

		void SetGraphicMatrix(mat4* gphmat);
		void GetWorldTransform(btScalar* transform);
		void setWorldTransform(const btTransform& centerOfMassWorldTrans) override;
	};
}