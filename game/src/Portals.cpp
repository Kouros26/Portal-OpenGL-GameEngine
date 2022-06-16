#include "../include/Portals.h"

Portals::Portals(std::string name, lm::vec3& pos)
{
	this->name = name;

	if (this->name == "Orange")
	{
		this->model = Singleton::resources->Get<Model>("OrangePortal");
		this->shader = Singleton::shaderList.at(0);
		this->pos = pos;
		this->rot = lm::vec3::zero;
		this->scale = lm::vec3(1, 1, 1);
		this->parent = nullptr;
		this->localTransform = lm::mat4::createTransformMatrix(pos, rot, scale);
		this->globalTransform = this->localTransform;
	}

	else if (this->name == "Blue")
	{
		this->model = Singleton::resources->Get<Model>("BluePortal");
		this->shader = Singleton::shaderList.at(0);
		this->pos = pos;
		this->rot = lm::vec3::zero;
		this->scale = lm::vec3(1, 1, 1);
		this->parent = nullptr;
		this->localTransform = lm::mat4::createTransformMatrix(pos, rot, scale);
		this->globalTransform = this->localTransform;
	}

	this->CreateBoxRigidBody(lm::vec3(0.5f, 0.5f, 0.5f), lm::vec3(1.f, 1.f, 1.f), 0);
	Singleton::sceneGraph.push_back(this);
}

void Portals::ProcessKeyboard(whichPortal color, lm::vec3& pos, float& deltaTime)
{
	if (color == BLUE && countBlue == 0)
	{
		blue = new Portals("Blue", pos);
		BlueFront = FindNormal(pos);
		if (BlueFront.X() == 1.f || BlueFront.X() == -1.f)
			blue->globalTransform = lm::mat4::lookAtRotation(*blue->GetPos(), *blue->GetPos() + BlueFront, lm::vec3::up).getInverse();
		countBlue = 1;
		return;
	}

	if (color == ORANGE && countOrange == 0)
	{
		orange = new Portals("Orange", pos);
		OrangeFront = FindNormal(pos);
		if (OrangeFront.X() == 1.f || OrangeFront.X() == -1.f)
			orange->globalTransform = lm::mat4::lookAtRotation(*orange->GetPos(), *orange->GetPos() + OrangeFront, lm::vec3::up).getInverse();
		countOrange = 1;
		return;
	}

	if (color == BLUE && countBlue == 1)
	{
		blue->RepositionRigidBody(pos);
		BlueFront = FindNormal(pos);
		if (BlueFront.X() == 1.f || BlueFront.X() == -1.f)
			blue->globalTransform = lm::mat4::lookAtRotation(*blue->GetPos(), *blue->GetPos() + BlueFront, lm::vec3::up).getInverse();
		return;
	}

	if (color == ORANGE && countOrange == 1)
	{
		orange->RepositionRigidBody(pos);
		OrangeFront = FindNormal(pos);
		if (OrangeFront.X() == 1.f || OrangeFront.X() == -1.f)
			orange->globalTransform = lm::mat4::lookAtRotation(*orange->GetPos(), *orange->GetPos() + OrangeFront, lm::vec3::up).getInverse();
		return;
	}

	//if (color == BLUE && countBlue == 1)
	//{
	//	blue->RepositionRigidBody(pos, pos);
	//	return;
	//}

	//if (color == ORANGE && countOrange == 1)
	//{
	//	orange->RepositionRigidBody(pos, pos);
	//}
}

void Portals::OnCollisionEnter(GameObject* me, GameObject* other, btPersistentManifold const* manifold)
{
	if (me->GetName() == "Blue")
		bluetp = true;

	if (me->GetName() == "Orange")
		orangetp = true;
}

void Portals::OnCollisionExit(GameObject* me, GameObject* other, btPersistentManifold const* manifold)
{
	if (me->GetName() == "Blue")
		bluetp = false;

	if (me->GetName() == "Orange")
		orangetp = false;
}

void Portals::RepositionRigidBody(vec3 const& newPosition)
{

	RigidBody* rb = (RigidBody*)GetRigidBody();
	if (rb == nullptr) return;

	Bullet::CustomMotionState* cust = rb->GetMS();

	btVector3 position(newPosition.X(), newPosition.Y(), newPosition.Z());

	btTransform initialTransform;

	initialTransform.setOrigin(position);

	//rb->GetRB()->setWorldTransform(initialTransform);
	cust->setWorldTransform(initialTransform);
}

lm::vec3 Portals::FindNormal(lm::vec3& pos)
{
	std::cout << "X : " << pos.X() << " Z : " << pos.Z() << std::endl;
	if (pos.Z() < -13.5f && pos.Z() >= - 14.f)
		return lm::vec3::backward;

	if (pos.Z() <= 14 && pos.Z() > 13.5)
		return lm::vec3::foward;

	if (pos.X() < -13.5f && pos.X() >= -14.f)
		return lm::vec3::right;

	if (pos.X() <= 44 && pos.X() > 43.5)
		return lm::vec3::left;

	return vec3(0.f);
}

float Portals::VecToDeg(lm::vec3& normal)
{
	Singleton::cam->GetYaw() = 0;
	Singleton::cam->GetPitch() = 0;

	//lm::vec3 frontnormal = Singleton::cam->GetFoward();

	//std::cout << " x : " << frontnormal.X() << " z : " << frontnormal.Z() << std::endl;

	//if (-frontnormal.X() > frontnormal.Z())
	//	frontnormal = lm::vec3::foward;
	//else if (-frontnormal.X() < frontnormal.Z())
	//	frontnormal = lm::vec3::backward;

	if (normal.X() == lm::vec3::foward.X() && normal.Z() == lm::vec3::foward.Z())
		return 90.f * 5;

	if (normal.X() == lm::vec3::backward.X() && normal.Z() == lm::vec3::backward.Z())
		return 90.f * 6;

	if (normal.X() == lm::vec3::left.X() && normal.Z() == lm::vec3::left.Z())
		return 90.f * 5;

	if (normal.X() == lm::vec3::right.X() && normal.Z() == lm::vec3::right.Z())
		return 90.f;

	return 0;
}

float Portals::GetWorldYaw()
{
	return Singleton::cam->GetYaw();
}

void Portals::Update(Player& player)
{

	for (int i = 0; i < Singleton::sceneGraph.size(); i++)
		if (Singleton::sceneGraph[i]->GetName() == "Blue")
			BlueposInGraph = i;

		else if (Singleton::sceneGraph[i]->GetName() == "Orange")
			OrangeposInGraph = i;

	if (bluetp)
	{
		//Singleton::cam->GetViewMatrix().yRotation(angle);
		player.RepositionRigidBody(*Singleton::sceneGraph.at(OrangeposInGraph)->GetPos() + OrangeFront * 1.2f, OrangeFront);
		std::cout << "x : " << Singleton::cam->GetFoward().X() << " z : " << Singleton::cam->GetFoward().Z() << std::endl;
		Singleton::cam->ProcessMouseMovement(VecToDeg(OrangeFront), 0);
	}

	if (orangetp)
	{
		player.RepositionRigidBody(*Singleton::sceneGraph.at(BlueposInGraph)->GetPos() + BlueFront * 1.2f, BlueFront);
		std::cout << "x : " << Singleton::cam->GetFoward().X() << " z : " << Singleton::cam->GetFoward().Z() << std::endl;
		Singleton::cam->ProcessMouseMovement(VecToDeg(BlueFront), 0);
	}
}
