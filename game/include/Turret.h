#pragma once
#include "Vec3/Vec3.h"
#include "../include/Singleton.h"

using namespace lm;

class Turret : public GameObject
{
private:
	Graph* turret;

public:
	Turret(Model* model, std::string shaderPath);
	void Update();
};