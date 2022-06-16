#include "..\..\include\Bullet\CustomMotionState.h"
#include "Vec4/Vec4.h"
#include "../../include/Bullet/WorldPhysicsComponents.h"

using namespace Bullet;

void Bullet::CustomMotionState::SetGraphicMatrix(mat4* gphmat)
{
	GraphicMatrix = gphmat;
}

void Bullet::CustomMotionState::GetWorldTransform(btScalar* transform)
{
	btTransform trans;
	getWorldTransform(trans);
	trans.getOpenGLMatrix(transform);
}

void Bullet::CustomMotionState::setWorldTransform(const btTransform& centerOfMassWorldTrans)
{
	btDefaultMotionState::setWorldTransform(centerOfMassWorldTrans);

	if (GraphicMatrix != nullptr)
	{
		btScalar worldMatrix[16];
		btTransform worldTrans;
		getWorldTransform(worldTrans);
		worldTrans.getOpenGLMatrix(worldMatrix);

		*GraphicMatrix = WorldPhysics::GetInstance().ScalarToMat4(worldMatrix);
	}
}