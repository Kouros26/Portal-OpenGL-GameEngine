#include "../include/Player.h"
#include "../include/Bullet/BoxRigidBody.h"
#include "../include/Bullet/CapsuleRigidBody.h"
#include "../include/Bullet/SphereRigidBody.h"
#include "../include/Bullet/CustomMotionState.h"
#include "../include/Singleton.h"
#include "../../libGL/include/Application.h"
#include "../include/Platform.h"
#include "../include/Companion.h"

Player::Player()
{
	/*Model* playerModel = new Model("Assets/Meshes/Ultimate Modular Sci-Fi/OBJ/Props_Computer.obj", Singleton::cam, Singleton::shaderList.at(0));
	playerGraph = new Graph(playerModel, Singleton::shaderList.at(0), lm::vec3::zero, lm::vec3::zero, lm::vec3(1.0f, 1.0f, 1.0f));
	Singleton::sceneGraph.push_back(playerGraph);*/
}

Player::Player(std::string name, Model* model, Shader* shader, lm::vec3 position, lm::vec3 rotation, lm::vec3 scale)
{
	/* GameObject */
	this->name = name;
	this->model = model;
	this->shader = shader;
	this->parent = parent;
	this->parent = nullptr;
	this->localTransform = lm::mat4::createTransformMatrix(position, rotation, scale);
	this->globalTransform = this->localTransform;
	this->pos = position;
	this->rot = rotation;
	this->scale = scale;
}

Player::Player(std::string name, Model* model, Shader* shader, lm::vec3 position, lm::vec3 rotation, lm::vec3 scale,
	Graph* parent, GLFWwindow* window)
{
	/* GameObject */
	this->name = name;
	this->model = model;
	this->shader = shader;
	this->parent = parent;
	this->parent->AddChild(this, window);
	this->localTransform = lm::mat4::createTransformMatrix(position, rotation, scale);
	this->globalTransform = this->parent->GetGlobalMatrix() * this->localTransform;
	this->pos = position;
	this->rot = rotation;
	this->scale = scale;
}

void Player::OnCollisionEnter(GameObject* me, GameObject* other, btPersistentManifold const* manifold)
{
	if (dynamic_cast<Platform*>(other) != nullptr)
	{
		if (manifold->getContactPoint(0).m_normalWorldOnB == btVector3(0, 1, 0))
		{
			RigidBody* rb = (RigidBody*)GetRigidBody();
			if (rb == nullptr)
				return;

			me->SetParent(other);
			vec3 pos = *me->GetPos() - *other->GetPos();
			this->localTransform = lm::mat4::createTransformMatrix(pos, me->GetRot(), me->GetScale());
		}
	}
}

void Player::OnCollisionExit(GameObject* me, GameObject* other, btPersistentManifold const* manifold)
{
	if (dynamic_cast<Platform*>(other) != nullptr)
	{
		RigidBody* rb = (RigidBody*)GetRigidBody();
		if (rb == nullptr)
			return;

		me->SetParent(nullptr);
		vec3 pos = *me->GetPos() + *other->GetPos();
		this->localTransform = lm::mat4::createTransformMatrix(pos, me->GetRot(), me->GetScale());
	}
}

void Player::Update(float deltaTime)
{
	vec3 NewPos = pos;
	NewPos.Y() += 1;
	Singleton::cam->GetPosition() = NewPos;
	Move(deltaTime);
	RotationOnY();
	UpdateCompanion();
}

void Player::Move(float deltaTime)
{
	btVector3 add(0, 0, 0);
	RigidBody* rb = (RigidBody*)GetRigidBody();
	if (rb == nullptr)
		return;

	vec3 movement(0, rb->GetRB()->getLinearVelocity().y() - (rb->GetMass() * deltaTime), 0);

	vec3 front = Singleton::cam->setFrontDirection();
	movement += front * Singleton::cam->GetMovementSpeed() * Singleton::cam->GetFrontInput();
	movement += Singleton::cam->GetRight() * Singleton::cam->GetMovementSpeed() * Singleton::cam->GetRightInput();


	if (Singleton::cam->GetUpInput() == 1)
		add = { 0, 0.5f,0 };

	if (this->parent == nullptr)
		rb->GetRB()->setLinearVelocity(btVector3(movement.X(), movement.Y(), movement.Z()) + add);
	else
	{
		this->localTransform[3][0] += movement.X() * deltaTime;
		//this->localTransform[3][1] += add.y();
		this->localTransform[3][2] += movement.Z() * deltaTime;
	}

	Singleton::cam->GetFrontInput() = 0;
	Singleton::cam->GetRightInput() = 0;
	Singleton::cam->ProcessKeyboard(JUMPSTOP, deltaTime);

	if (!Singleton::cam->GetInAir())
		Application::getJumpButton() = false;
}

vec3 Player::GetPickingRay(int x, int y)
{
	// calculate the field-of-view
	float tanFov = 1.0f / Singleton::cam->GetNear();
	float fov = btScalar(2.0) * btAtan(tanFov);

	// get a ray pointing forward from the
	// camera and extend it to the far plane
	vec3 rayFrom = Singleton::cam->GetPosition();
	vec3 rayForward = (Singleton::cam->GetFoward() - Singleton::cam->GetPosition());
	rayForward.normalize();
	rayForward *= Singleton::cam->GetFar();

	// find the horizontal and vertical vectors
	// relative to the current camera view
	vec3 ver = Singleton::cam->GetUp();
	vec3 hor = rayForward.crossProduct(ver);
	hor.normalize();
	ver = hor.crossProduct(rayForward);
	ver.normalize();
	hor *= 2.f * Singleton::cam->GetFar() * tanFov;
	ver *= 2.f * Singleton::cam->GetFar() * tanFov;

	// calculate the aspect ratio
	Application* app = (Application*)glfwGetWindowUserPointer(Singleton::window);
	float aspect = app->GetScrWdth() / app->GetScrHght();

	// adjust the forward-ray based on
	// the X/Y coordinates that were clicked
	hor *= aspect;
	vec3 rayToCenter = rayFrom + rayForward;
	vec3 dHor = hor * 1.f / float(app->GetScrWdth());
	vec3 dVert = ver * 1.f / float(app->GetScrHght());
	vec3 rayTo = rayToCenter - 0.5f * hor + 0.5f * ver;
	rayTo += btScalar(x) * dHor;
	rayTo -= btScalar(y) * dVert;

	// return the final result
	return rayTo;
}

bool Player::Raycast(vec3 const& startPos, vec3 const& direction, RayResult& output)
{
	if (!WorldPhysics::GetInstance().GetDiscretWorld())
		return false;

	//get picking ray from where we clicked
	btVector3 rayTo(direction.X(), direction.Y(), direction.Z());
	btVector3 rayFrom(startPos.X(), startPos.Y(), startPos.Z());

	//create raycast callback object
	btCollisionWorld::ClosestRayResultCallback rayCallback(rayFrom, rayTo);

	//perform rayCast
	WorldPhysics::GetInstance().GetDiscretWorld()->rayTest(rayFrom, rayTo, rayCallback);

	//hit something ?
	if (rayCallback.hasHit())
	{
		//get btRigidbody
		btRigidBody* body = (btRigidBody*)btRigidBody::upcast(rayCallback.m_collisionObject);
		if (!body)
			return false;

		//prevent us from picking kinematic object
		if (body->isKinematicObject())
			return false;

		//srt the result data

		output._gameObject = (GameObject*)body->getUserPointer();
		output._rb = body;
		WorldPhysics::GetInstance().hitpoint = rayTo;
		WorldPhysics::GetInstance().campos = rayFrom;
		output._hitPoint = vec3(rayCallback.m_hitPointWorld.x(), rayCallback.m_hitPointWorld.y(), rayCallback.m_hitPointWorld.z());

		return true;
	}

	//nothing hit
	return false;
}

void Player::RotationOnY()
{
	vec3 rot = Singleton::cam->GetViewMatrix().extractRotation(Singleton::cam->GetViewMatrix().extractScale());
	vec3 pos = this->pos;

	RepositionRigidBody(pos, vec3(0, 0, 0));
}

bool Player::InRangeOfCompanionCube(RayResult& result)
{
	if (_holdCompanion)
		return false;

	if (dynamic_cast<Companion*>(result._gameObject) == nullptr)
		return false;

	vec3 hitpoint = result._hitPoint;
	vec3 pos = this->pos;
	float distance = sqrt((hitpoint.X() - pos.X()) * (hitpoint.X() - pos.X()) +
		(hitpoint.Y() - pos.Y()) * (hitpoint.Y() - pos.Y()) +
		(hitpoint.Z() - pos.Z()) * (hitpoint.Z() - pos.Z()));

	if (distance > this->_RangeToGrabCompanion)
		return false;

	return true;
}

void Player::GrabbCompanionCube(RayResult& result)
{
	_holdCompanion = true;
	result._gameObject->SetParent(this);
	_lastCompanionHold = dynamic_cast<Companion*>(result._gameObject);
	_childCompanionPos = *result._gameObject->GetPos() - *this->GetPos();
	result._gameObject->GetLocalMatrix() = lm::mat4::createTransformMatrix(_childCompanionPos, result._gameObject->GetRot(), result._gameObject->GetScale());
}

void Player::UnGrabbCompanionCube(RayResult& result)
{
	_holdCompanion = false;
	_lastCompanionHold->SetParent(nullptr);
	vec3 pos = *_lastCompanionHold->GetPos() + *this->GetPos();
	result._gameObject->GetLocalMatrix() = lm::mat4::createTransformMatrix(pos, _lastCompanionHold->GetRot(), _lastCompanionHold->GetScale());
}

void Player::UpdateCompanion()
{
	if (!_holdCompanion) return;

	_lastCompanionHold->RepositionRigidBody(*_lastCompanionHold->GetPos(), vec3(0));
	_lastCompanionHold->GetLocalMatrix()[3][0] = Singleton::cam->GetFoward().X() * 3;
	_lastCompanionHold->GetLocalMatrix()[3][1] = Singleton::cam->GetFoward().Y() * 3;
	_lastCompanionHold->GetLocalMatrix()[3][2] = Singleton::cam->GetFoward().Z() * 3;
}
