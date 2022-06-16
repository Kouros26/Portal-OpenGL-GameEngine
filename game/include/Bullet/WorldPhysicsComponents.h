#pragma once

#include <vector>
#include <set>
#include <algorithm> // std::set_symmetric_difference
#include <functional>

#include "btBulletCollisionCommon.h"
#include "btBulletDynamicsCommon.h"
#include "Vec3/Vec3.h"
#include "CustomMotionState.h"
#include "../GameObject.h"

using namespace lm;
using namespace Bullet;

namespace Bullet
{
	class WorldPhysics
	{
		btBroadphaseInterface*		_broadPhase = nullptr;
		btCollisionConfiguration*	_collisionConfiguration = nullptr;
		btCollisionDispatcher*		_dispatcher = nullptr;
		btConstraintSolver*			_solver = nullptr;
		btDynamicsWorld*			_world = nullptr;

		std::vector<btRigidBody*> _bodies;
		std::vector<GameObject*> _gameObject;


		static void						TickCallBack(btDynamicsWorld* world, btScalar timeStep);
		using CollisionSet = std::set< std::tuple<btCollisionObject const*, btCollisionObject const*, btPersistentManifold const*>>;
		CollisionSet LastTickCollisionPairs;

	public:
		WorldPhysics(WorldPhysics const&) = delete;

		static WorldPhysics&			GetInstance();
		std::vector<btRigidBody*>&		GetBodies() { return _bodies; }
		std::vector<GameObject*>&		GetGameObjects() { return _gameObject; }
		btDynamicsWorld*				GetDiscretWorld() { return _world; }

		void							AddRigidbodyInWorld(btRigidBody* rb);
		void							RemoveRigidbodyFromWorld(btRigidBody* rb);
		mat4							ScalarToMat4(btScalar* btsc);
		void							checkCollision();
		btRigidBody*					addShape(float rad, Vec3<float> const& pos, float mass);
		btRigidBody*					CreateRigidBody(btCollisionShape* shape, float mass, CustomMotionState* custMS, btVector3 inertia);
		void							UpdatePhysics(float dt);
		btVector3 hitpoint;
		btVector3 campos;
	protected:

		WorldPhysics();
	};

	using CollisionEnterCallBack = std::function<void(GameObject*, GameObject*, btPersistentManifold const*)>;
	using CollisionExitCallBack = std::function<void(GameObject*, GameObject*, btPersistentManifold const*)>;

	struct CollisionCallBacks
	{
		CollisionEnterCallBack OnEnter;
		CollisionExitCallBack OnExit;
	};
}