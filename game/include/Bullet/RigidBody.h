#pragma once

#include <functional>
#include "Vec3/Vec3.h"
#include "WorldPhysicsComponents.h"
#include "../../game/include/Component.h"

namespace Bullet
{

	enum class rbState
	{
		DYNAMIC,
		KINETIC,
		STATIC
	};

	enum class RigidBodyType
	{
		BOX,
		CAPSULE,
		SPHERE
	};

	class RigidBody : public Component
	{
	protected:
		btRigidBody* _body = nullptr;
		CustomMotionState* _ms = nullptr;
		btCollisionShape* _rbShape = nullptr;
		CollisionCallBacks* _cb = nullptr;
		RigidBodyType _rbType;
		btScalar _mass;

	public:
		void				SetRBState(rbState newState);
		void				SetMotionState(CustomMotionState* ms) { _ms = ms; }

		btRigidBody*		GetRB() { return _body; }
		CustomMotionState*	GetMS() { return _ms; }
		btCollisionShape*	GetRBShape() { return _rbShape; }
		CollisionCallBacks* GetCallBack() { return _cb; }
		RigidBodyType&		GetShapeType() { return _rbType; }
		btScalar&			GetMass() { return _mass; }
	};
}