#include "../../include/Bullet/WorldPhysicsComponents.h"
#include "../../include/Bullet/RigidBody.h"
#include <iterator>
using namespace Bullet;

WorldPhysics& Bullet::WorldPhysics::GetInstance()
{
	static WorldPhysics instance;
	return instance;
}

void Bullet::WorldPhysics::AddRigidbodyInWorld(btRigidBody* rb)
{
	_world->addRigidBody(rb);

	if (std::find(_bodies.begin(), _bodies.end(), rb) != _bodies.end())
		_bodies.push_back(rb);
}

void Bullet::WorldPhysics::RemoveRigidbodyFromWorld(btRigidBody* rb)
{
	_world->removeRigidBody(rb);
}

mat4 Bullet::WorldPhysics::ScalarToMat4(btScalar* btsc)
{
	Vec4<float> r1(btsc[0], btsc[1], btsc[2], btsc[3]);
	Vec4<float> r2(btsc[4], btsc[5], btsc[6], btsc[7]);
	Vec4<float> r3(btsc[8], btsc[9], btsc[10], btsc[11]);
	Vec4<float> r4(btsc[12], btsc[13], btsc[14], btsc[15]);

	return mat4(r1, r2, r3, r4);
}

void Bullet::WorldPhysics::TickCallBack(btDynamicsWorld* world, btScalar timeStep)
{
	WorldPhysics* thisWorld = (WorldPhysics*)world->getWorldUserInfo();

	thisWorld->checkCollision();
}

void Bullet::WorldPhysics::checkCollision()
{
	if (_dispatcher == nullptr)
		return;

	CollisionSet newCollisions;

	for (int i = 0; i < _dispatcher->getNumManifolds(); ++i)
	{
		btPersistentManifold* manifold = _dispatcher->getManifoldByIndexInternal(i);
		if (manifold == nullptr || manifold->getNumContacts() <= 0)
			continue;

		const btCollisionObject* body0 = manifold->getBody0();
		const btCollisionObject* body1 = manifold->getBody1();
		if (body0 > body1)
			std::swap(body0, body1);

		newCollisions.emplace(body0, body1, manifold);
	}

	CollisionSet diffenrenceSet;
	std::set_symmetric_difference(LastTickCollisionPairs.begin(), LastTickCollisionPairs.end(),
		newCollisions.begin(), newCollisions.end(),
		std::inserter(diffenrenceSet, diffenrenceSet.begin()));

	for (std::tuple<const btCollisionObject*, const btCollisionObject*, const btPersistentManifold*> const& pair : diffenrenceSet)
	{
		if (LastTickCollisionPairs.find(pair) == LastTickCollisionPairs.end())
		{
			//nouvelle collision
			GameObject* go0 = (GameObject*)std::get<0>(pair)->getUserPointer();
			GameObject* go1 = (GameObject*)std::get<1>(pair)->getUserPointer();
			btRigidBody* rb0 = (btRigidBody*)std::get<0>(pair);
			btRigidBody* rb1 = (btRigidBody*)std::get<1>(pair);

			if (!go0 || !go1)
				continue;

			RigidBody* tmp0 = (RigidBody*)go0->GetRigidBody();
			RigidBody* tmp1 = (RigidBody*)go1->GetRigidBody();
			CollisionCallBacks* callBacks = tmp0->GetCallBack();
			CollisionCallBacks* callBacks2 = tmp1->GetCallBack();

			if (callBacks->OnEnter)
				callBacks->OnEnter(go0, go1, std::get<2>(pair));

			if(callBacks2->OnEnter)
				callBacks2->OnEnter(go1, go0, std::get<2>(pair));
		}
		else
		{
			//collision end
			GameObject* go0 = (GameObject*)std::get<0>(pair)->getUserPointer();
			GameObject* go1 = (GameObject*)std::get<1>(pair)->getUserPointer();
			btRigidBody* rb0 = (btRigidBody*)std::get<0>(pair);
			btRigidBody* rb1 = (btRigidBody*)std::get<1>(pair);

			if (!go0 || !go1)
				continue;

			RigidBody* tmp0 = (RigidBody*)go0->GetRigidBody();
			RigidBody* tmp1 = (RigidBody*)go1->GetRigidBody();
			CollisionCallBacks* callBacks = tmp0->GetCallBack();
			CollisionCallBacks* callBacks2 = tmp1->GetCallBack();

			if (callBacks->OnExit)
				callBacks->OnExit(go0, go1, std::get<2>(pair));

			if (callBacks2->OnExit)
				callBacks2->OnExit(go1, go0, std::get<2>(pair));
		}
	}

	// mettre a jour le collision cache
	LastTickCollisionPairs = newCollisions;
}

btRigidBody* Bullet::WorldPhysics::addShape(float rad, Vec3<float> const& pos, float mass)
{
	btTransform t;
	t.setIdentity();
	t.setOrigin(btVector3(pos.X(), pos.Y(), pos.Z()));
	btSphereShape* shape = new btSphereShape(rad);
	btVector3 inertia(0, 0, 0);

	if (mass != 0)
		shape->calculateLocalInertia(mass, inertia);

	btMotionState* motion = new btDefaultMotionState(t);
	btRigidBody::btRigidBodyConstructionInfo info(mass, motion, shape, inertia);
	btRigidBody* body = new btRigidBody(info);
	_world->addRigidBody(body);
	_bodies.push_back(body);

	return body;
}

btRigidBody* Bullet::WorldPhysics::CreateRigidBody(btCollisionShape* shape, float mass, CustomMotionState* custMS, btVector3 inertia)
{
	btRigidBody::btRigidBodyConstructionInfo info(mass, custMS, shape, inertia);
	btRigidBody* body = new btRigidBody(info);

	body->setActivationState(DISABLE_DEACTIVATION);
	_world->addRigidBody(body);
	_bodies.push_back(body);

	return body;
}

void Bullet::WorldPhysics::UpdatePhysics(float dt)
{
	if (_world != nullptr)
		_world->stepSimulation(dt); //divide by 1000 to convert ms to s
}

Bullet::WorldPhysics::WorldPhysics()
{
	_collisionConfiguration = new btDefaultCollisionConfiguration();
	_dispatcher = new btCollisionDispatcher(_collisionConfiguration);
	_broadPhase = new btDbvtBroadphase();
	_solver = new btSequentialImpulseConstraintSolver();
	_world = new btDiscreteDynamicsWorld(_dispatcher, _broadPhase, _solver, _collisionConfiguration);

	_world->setGravity(btVector3(0, -9.8f, 0));

	/* Collisions manifold */
	_world->setInternalTickCallback(WorldPhysics::TickCallBack, this);
}
