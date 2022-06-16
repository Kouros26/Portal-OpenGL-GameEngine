#include "..\..\include\Bullet\RigidBody.h"

void Bullet::RigidBody::SetRBState(rbState newState)
{
	switch (newState)
	{
	case rbState::DYNAMIC:
		_body->setCollisionFlags(_body->getCollisionFlags() | btCollisionObject::CF_DYNAMIC_OBJECT);
		break;
	case rbState::KINETIC:
		_body->setCollisionFlags(_body->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
		break;
	case rbState::STATIC:
		_body->setCollisionFlags(_body->getCollisionFlags() | btCollisionObject::CF_STATIC_OBJECT);
		break;
	default:
		break;
	}
}
