#pragma once
#include "Player.h"
#include "../include/Singleton.h"

class Gun : public GameObject
{

public:

	Gun(Graph* player, GLFWwindow* window);

	void Update();
};