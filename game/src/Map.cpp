#include "../include/Map.h"
#include "../include/Bullet/RigidBody.h"

void Map::InitWalls()
{
	model = Singleton::resources->Get<Model>("Assets/Meshes\\Ultimate_Modular_Sci-Fi\\OBJ\\LongWindow_Wall_SideB.obj");
	this->wallLeft = new GameObject("LeftWall", model, Singleton::shaderList.at(0), lm::vec3(-15, -1, 0), lm::vec3(0, 90, 0), lm::vec3(7.5f, 7.5f, 7.5f));
	wallLeft->CreateBoxRigidBody(lm::vec3(0.5f), lm::vec3(2, 50, 90), 0);
	dynamic_cast<Bullet::RigidBody*>(wallLeft->GetComponents()[0])->SetRBState(Bullet::rbState::STATIC);

	this->wallRight = new GameObject("RightWall", model, Singleton::shaderList.at(0), lm::vec3(45, -1, 0), lm::vec3(0, -90, 0), lm::vec3(7.5f, 7.5f, 7.5f));
	wallRight->CreateBoxRigidBody(lm::vec3(0.5f), lm::vec3(2, 50, 90), 0);
	dynamic_cast<Bullet::RigidBody*>(wallRight->GetComponents()[0])->SetRBState(Bullet::rbState::STATIC);

	this->wallFront = new GameObject("FrontWall", model, Singleton::shaderList.at(0), lm::vec3(15, -1, -15), lm::vec3(0, 0, 0), lm::vec3(15, 7.5f, 15));
	wallFront->CreateBoxRigidBody(lm::vec3(0.5f), lm::vec3(90, 50, 2), 0);
	dynamic_cast<Bullet::RigidBody*>(wallFront->GetComponents()[0])->SetRBState(Bullet::rbState::STATIC);

	this->wallBack = new GameObject("BackWall", model, Singleton::shaderList.at(0), lm::vec3(15, -1, 15), lm::vec3(0, 180, 0), lm::vec3(15, 7.5f, 15));
	wallBack->CreateBoxRigidBody(lm::vec3(0.5f), lm::vec3(90, 50, 2), 0);
	dynamic_cast<Bullet::RigidBody*>(wallBack->GetComponents()[0])->SetRBState(Bullet::rbState::STATIC);
}

void Map::InitFloorNRoof()
{
	model = Singleton::resources->Get<Model>("Assets/Meshes\\Ultimate_Modular_Sci-Fi\\OBJ\\FloorTile_Double_Hallway.obj");
	this->floor = new GameObject("Floor", model, Singleton::shaderList.at(0), lm::vec3(0, -1, 0), lm::vec3(0, 0, 0), lm::vec3(15, 15, 15));
	floor->CreateBoxRigidBody(lm::vec3(0.5f), lm::vec3(90, 0.5f, 30), 0);
	dynamic_cast<Bullet::RigidBody*>(floor->GetComponents()[0])->SetRBState(Bullet::rbState::STATIC);

	model = Singleton::resources->Get<Model>("Assets/Meshes\\Ultimate_Modular_Sci-Fi\\OBJ\\RoofTile_Details.obj");
	this->roofL = new GameObject("LeftRoof", model, Singleton::shaderList.at(0), lm::vec3(0, 27.5f, 0), lm::vec3(180, 0, 0), lm::vec3(15, 15, 12.5f));
	roofL->CreateBoxRigidBody(lm::vec3(0.5f), lm::vec3(30, 0.5f, 25), 0);
	dynamic_cast<Bullet::RigidBody*>(roofL->GetComponents()[0])->SetRBState(Bullet::rbState::STATIC);

	this->roofR = new GameObject("RightRoof", model, Singleton::shaderList.at(0), lm::vec3(30, 27.5f, 0), lm::vec3(180, 0, 0), lm::vec3(15, 15, 12.5f));
	roofR->CreateBoxRigidBody(lm::vec3(0.5f), lm::vec3(30, 0.5f, 25), 0);
	dynamic_cast<Bullet::RigidBody*>(roofR->GetComponents()[0])->SetRBState(Bullet::rbState::STATIC);
}

void Map::InitColumns()
{
	model = Singleton::resources->Get<Model>("Assets/Meshes\\Ultimate_Modular_Sci-Fi\\OBJ\\Column_3.obj");
	this->columnFL = new GameObject("FrontLeftColumn", model, Singleton::shaderList.at(0), lm::vec3(-15.5f, -1, -15.5f), lm::vec3(0, 0, 0), lm::vec3(7.5f, 7.5f, 7.5f));
	columnFL->CreateBoxRigidBody(lm::vec3(0.5f), lm::vec3(5, 50, 5), 0);
	dynamic_cast<Bullet::RigidBody*>(columnFL->GetComponents()[0])->SetRBState(Bullet::rbState::STATIC);

	this->columnFR = new GameObject("FrontRightColumn", model, Singleton::shaderList.at(0), lm::vec3(45.5f, -1, -15.5f), lm::vec3(0, 0, 0), lm::vec3(7.5f, 7.5f, 7.5f));
	columnFR->CreateBoxRigidBody(lm::vec3(0.5f), lm::vec3(5, 50, 5), 0);
	dynamic_cast<Bullet::RigidBody*>(columnFR->GetComponents()[0])->SetRBState(Bullet::rbState::STATIC);

	this->columnBL = new GameObject("BackLeftColumn", model, Singleton::shaderList.at(0), lm::vec3(-15.5f, -1, 15.5f), lm::vec3(0, 0, 0), lm::vec3(7.5f, 7.5f, 7.5f));
	columnBL->CreateBoxRigidBody(lm::vec3(0.5f), lm::vec3(5, 50, 5), 0);
	dynamic_cast<Bullet::RigidBody*>(columnBL->GetComponents()[0])->SetRBState(Bullet::rbState::STATIC);

	this->columnBR = new GameObject("BackRightColumn", model, Singleton::shaderList.at(0), lm::vec3(45.5f, -1, 15.5f), lm::vec3(0, 0, 0), lm::vec3(7.5f, 7.5f, 7.5f));
	columnBR->CreateBoxRigidBody(lm::vec3(0.5f), lm::vec3(5, 50, 5), 0);
	dynamic_cast<Bullet::RigidBody*>(columnBR->GetComponents()[0])->SetRBState(Bullet::rbState::STATIC);
}

void Map::PushGraph()
{
	Singleton::sceneGraph.push_back(this->floor);
	Singleton::sceneGraph.push_back(this->wallLeft);
	Singleton::sceneGraph.push_back(this->wallRight);
	Singleton::sceneGraph.push_back(this->wallFront);
	Singleton::sceneGraph.push_back(this->wallBack);
	Singleton::sceneGraph.push_back(this->columnFL);
	Singleton::sceneGraph.push_back(this->columnFR);
	Singleton::sceneGraph.push_back(this->columnBL);
	Singleton::sceneGraph.push_back(this->columnBR);
	Singleton::sceneGraph.push_back(this->roofL);
	Singleton::sceneGraph.push_back(this->roofR);
}

Map::Map()
{
	InitFloorNRoof();
	InitWalls();
	InitColumns();

	PushGraph();
}
