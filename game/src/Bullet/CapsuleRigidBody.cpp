#include "..\..\include\Bullet\CapsuleRigidBody.h"

Bullet::CapsuleRigidBody::CapsuleRigidBody(float rad, float height, Vec3<float> const& pos, float mass, GameObject* futurAssignedGO)
{
	this->name = "CapsuleRigidBody";
	/* Create Shape */
	btCapsuleShape* shape = new btCapsuleShape(rad, height);
	btVector3 inertia(0, 0, 0);

	if (mass != 0)
		shape->calculateLocalInertia(mass, inertia);

	/* Create Motion State */
	btTransform t;
	t.setIdentity();
	t.setOrigin(btVector3(pos.X(), pos.Y(), pos.Z()));
	CustomMotionState* cust = new CustomMotionState(t);

	/* Create RigidBody */
	btRigidBody::btRigidBodyConstructionInfo info(mass, cust, shape, inertia);
	btRigidBody* body = new btRigidBody(info);

	/* Assign Function to be called for enter/exit collision */
	std::function<void(GameObject*, GameObject*, btPersistentManifold const*)> enter =
		[futurAssignedGO](GameObject* me, GameObject* other, btPersistentManifold const* manifold)
	{

		futurAssignedGO->OnCollisionEnter(me, other, manifold);
	};

	std::function<void(GameObject*, GameObject*, btPersistentManifold const*)> exit =
		[futurAssignedGO](GameObject* me, GameObject* other, btPersistentManifold const* manifold)
	{
		futurAssignedGO->OnCollisionExit(me, other, manifold);
	};

	CollisionCallBacks* cb = new CollisionCallBacks();
	cb->OnEnter = CollisionEnterCallBack(enter);
	cb->OnExit = CollisionEnterCallBack(exit);
	body->setUserPointer(futurAssignedGO);

	/* Assign Value */
	WorldPhysics::GetInstance().AddRigidbodyInWorld(body);
	_cb = cb;
	_body = body;
	_rbShape = shape;
	_ms = cust;
	_mass = mass;

	_rad = rad;
	_height = height;
}
