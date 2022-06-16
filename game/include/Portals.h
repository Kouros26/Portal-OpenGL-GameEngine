#pragma once
#include "Singleton.h"
#include "Player.h"
#include "Bullet/RigidBody.h"

enum whichPortal
{
	BLUE,
	ORANGE
};

class Portals : public GameObject
{
private:

	inline static Portals* blue = nullptr;
	inline static Portals* orange = nullptr;

	inline static int countBlue = 0, countOrange = 0;
	inline static int BlueposInGraph, OrangeposInGraph;
	inline static lm::vec3 BlueFront,OrangeFront;
	bool bluetp = false, orangetp = false;
	btRigidBody* rb;

public:
	Portals(std::string name, lm::vec3& pos);

	static void ProcessKeyboard(whichPortal color, lm::vec3& pos, float& deltaTime);
	void OnCollisionEnter(GameObject* me, GameObject* other, btPersistentManifold const* manifold) override;
	void OnCollisionExit(GameObject* me, GameObject* other, btPersistentManifold const* manifold) override;
	void RepositionRigidBody(vec3 const& newPosition);

	static lm::vec3 FindNormal(lm::vec3& pos);
	float VecToDeg(lm::vec3& normal);
	float GetWorldYaw();

	void Update(Player& player);
};
