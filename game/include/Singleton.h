#pragma once
#include "../../libGL/include/Ressource/AssimpShader.h"
#include "Player.h"

class Singleton
{
public:
	inline static Camera* cam;
	inline static Player* _player;
	inline static std::vector<Shader*> shaderList;
	inline static std::vector<Graph*> sceneGraph;
	inline static ResourceManager* resources;
	inline static GLFWwindow* window;

	Singleton();
};