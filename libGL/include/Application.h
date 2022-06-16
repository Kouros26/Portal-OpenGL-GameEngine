#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "../../game/include/UIManager.h"
#include "Core/Debug/Log.h"
#include <iostream>
#include <filesystem>
#include "LowRenderer/DirectionalLight.h"
#include "LowRenderer/PointLight.h"
#include "LowRenderer/SpotLight.h"
#include "../include/Ressource/AssimpModel.h"
#include "../include/Ressource/AssimpShader.h"
#include "../include/LowRenderer/AssimpMesh.h"
#include "../RapidJASON/include/rapidjson/stringbuffer.h"
#include "../RapidJASON/include/rapidjson/prettywriter.h"
#include "../../game/include/Singleton.h"
#include "../../game/include/Turret.h"
#include "../../game/include/Map.h"
#include "../../game/include/GameObject.h"
#include "../IrrKlang/include/irrKlang.h"

class Application
{
	GLFWwindow* window;
	UIManager* ui;
	irrklang::ISoundEngine* soundEngine;
	std::vector<DirectionalLight*> dirLights;
	std::vector<PointLight*> pointLights;
	std::vector<SpotLight*> spotLights;
	std::string currentUsedSafeFile;
	std::string lastUsedSafeFile;

	float SCREENWIDTH;
	float SCREENHEIGHT;
	const float cameraSpeed = 2.5f;
	float deltaTime = 0.0f;
	float lastFrame = 0.0f;
	float timeScale = 1.0f;
	bool sceneGraphActive;
	bool EditorModeActive = false;
	bool EditorButtonPressed = false;
	bool DebugDrawerButtonPressed = false;
	bool DebugDrawerActive = false;
	bool GrabbRaycastButtonPressed = false;
	inline static bool JumpButtonPressed = false;
	bool SoundButtonPressed = false;
	bool SoundPaused = false;
	float cooldownblue = 0.0f;
	float cooldownorange = 0.0f;

public :

	float yaw = 0;
	float pitch = 0;
	float lastX;
	float lastY;
	bool firstMouse;

	Application(Camera* cam);
	void Gameloop();
	void processInput(GLFWwindow* window);
	ResourceManager* GetResourceManager();
	GLFWwindow* GetWindow();
	void SetCamera(Camera* cam);
	void UpdateLights(Camera* cam, ResourceManager* manager);
	void UpdateScene(float deltaTime);
	void LoadModels();
	void EditorMode();
	void TerminateImGui();

	float GetScrWdth() { return SCREENWIDTH; }
	float GetScrHght() { return SCREENHEIGHT; }
	float GetDeltaTime();
	std::string& GetSaveFileName() { return currentUsedSafeFile; }
	std::vector<DirectionalLight*>& getDirLights();
	std::vector<PointLight*>& getPointLights();
	std::vector<SpotLight*>& getSpotLights();
	static std::vector<Graph*>& getSceneGraph();
	static std::vector<Shader*>& getShaderList();
	static Camera* getCamera();
	static bool& getJumpButton();

	static void framebuffer_size_callback(GLFWwindow* window, int width, int height);

	static void mouse_callback(GLFWwindow* window, double xpos, double ypos);

	void SaveScene();
	void SaveScene(std::string filePath);
	void NewScene(std::string filePath);
	bool ReadScene(char const* filepath);
	bool CreateSceneObject(std::fstream& file, std::vector<Graph*>& childs);
	bool CreatePointLight(std::fstream& file, lm::vec3& ambiantColor, lm::vec3& diffuseColor, lm::vec3& specularColor, float& constant, float& linear, float& quadratic);
	bool CreateSpotLight(std::fstream& file, lm::vec3& ambiantColor, lm::vec3& diffuseColor, lm::vec3& specularColor, float& constant, float& linear, float& quadratic, lm::vec3& direction, float cutOff, float outerCutOff);
	bool CreateBoxRigidBody(std::fstream& file, float& mass, lm::vec3& halfExtend, lm::vec3& scale);
	bool CreateCapsuleRigidBody(std::fstream& file, float& mass, float& radius, float& height);
	bool CreateSphereRigidBody(std::fstream& file, float& mass, float& radius);
};