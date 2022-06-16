#pragma once
#include "../include/Application.h"
#include "../../game/include/Bullet/WorldPhysicsComponents.h"
#include "../../game/include/Bullet/BoxRigidBody.h"
#include "../../game/include/Player.h"
#include "../../game/include/imgui_app.h"
#include "../../game/include/Portals.h"
#include "../../game/include/Platform.h"
#include "../../game/include/Bullet/DebugDrawer.h"
#include "../../game/include/Bullet/RigidBody.h"
#include "../../game/include/Player.h"
#include "../../game/include/Companion.h"
#include <stb_image.h>
#include "../../game/include/Gun.h"

using namespace Bullet;

Application::Application(Camera* cam)
{
	// settings
	SCREENWIDTH = 800;
	SCREENHEIGHT = 600;
	float lastX = SCREENWIDTH / 2.0f;
	float lastY = SCREENHEIGHT / 2.0f;
	bool firstMouse = true;

	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

	// glfw window creation
	// --------------------
	window = glfwCreateWindow(SCREENWIDTH, SCREENHEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
	}

	// IrrKlang : initialize
	soundEngine = irrklang::createIrrKlangDevice();

	Singleton::resources = new ResourceManager();

	Singleton::window = this->window;

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glfwSetWindowUserPointer(window, this);

	stbi_set_flip_vertically_on_load(true);

	SetCamera(cam);
	LoadModels();

	this->ui = new UIManager(window, EditorModeActive);
}

void Application::Gameloop()
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	WorldPhysics& world = WorldPhysics::GetInstance();

	SpotLight* spot = new SpotLight(lm::vec3(0.3f, 0.3f, 0.3f), lm::vec3(0.8f, 0.8f, 0.8f), lm::vec3(1.0f, 1.0f, 1.0f), Singleton::cam->GetPosition(), Singleton::cam->GetFoward(), std::cos(lm::vec3::degreesToRadians(12.5f)), std::cos(lm::vec3::degreesToRadians(15.0f)), 1.0f, 0.09f, 0.032f);
	//DirectionalLight* dir = new DirectionalLight(lm::vec3(0.3f, 0.3f, 0.3f), lm::vec3(0.8f, 0.8f, 0.8f), lm::vec3(1.0f, 1.0f, 1.0f), lm::vec3(0, -90, 0));
	getSpotLights().push_back(spot);
	//getDirLights().push_back(dir);

	Shader* debugShader = Singleton::resources->Create<Shader>("DebugDrawerShader", "libGL/shaders/DebugDrawVertexShader.glsl", "libGL/shaders/DebugDrawFragmentShader.glsl");
	DebugDrawer* db = new DebugDrawer(1, debugShader);
	WorldPhysics::GetInstance().GetDiscretWorld()->setDebugDrawer(db);

	//Model* cubeModel = resources->Create<Model>("Assets/Meshes/cube.obj");
	//Shader* lightShader = resources->Create<Shader>("WhiteShader", "libGL/shaders/LightVertexShader.glsl", "libGL/shaders/LightFragShader.glsl");
	//Texture* lightTex = resources->Create<Texture>("WhiteTexture", "Assets/Textures/plain-white-background.jpg", true);
	//Mesh* lightMesh = resources->Create<Mesh>("LightCube", cubeModel, lightShader, lightTex, cam);

	Shader* lightShader = Singleton::resources->Create<Shader>("LitShader", "libGL/shaders/LightVertexShader.glsl", "libGL/shaders/LightFragShader.glsl");
	Singleton::shaderList.push_back(lightShader);
	Model* cubeModel = Singleton::resources->Get<Model>("Assets/Meshes\\cube.obj");


	GameObject* light1 = new GameObject("LightCube1", cubeModel, lightShader, lm::vec3(-15.f, 15.f, -15.f), lm::vec3(0, 0, 0), lm::vec3(0.25f, 0.25f, 0.25f));
	light1->CreatePointLight(lm::vec3(1.0f, 1.0f, 1.0f), lm::vec3(0.8f, 0.8f, 0.8f), lm::vec3(1.0f, 1.0f, 1.0f), 1.0f, 0.09f, 0.032f);

	GameObject* light2 = new GameObject("LightCube2", cubeModel, lightShader, lm::vec3(45.f, 15.f, -15.f), lm::vec3(0, 0, 0), lm::vec3(0.25f, 0.25f, 0.25f));
	light2->CreatePointLight(lm::vec3(1.0f, 1.0f, 1.0f), lm::vec3(0.8f, 0.8f, 0.8f), lm::vec3(1.0f, 1.0f, 1.0f), 1.0f, 0.09f, 0.032f);

	GameObject* light3 = new GameObject("LightCube3", cubeModel, lightShader, lm::vec3(-15.f, 15.f, 15.f), lm::vec3(0, 0, 0), lm::vec3(0.25f, 0.25f, 0.25f));
	light3->CreatePointLight(lm::vec3(1.0f, 1.0f, 1.0f), lm::vec3(0.8f, 0.8f, 0.8f), lm::vec3(1.0f, 1.0f, 1.0f), 1.0f, 0.09f, 0.032f);

	GameObject* light4 = new GameObject("LightCube4", cubeModel, lightShader, lm::vec3(45.f, 15.f, 15.f), lm::vec3(0, 0, 0), lm::vec3(0.25f, 0.25f, 0.25f));
	light4->CreatePointLight(lm::vec3(1.0f, 1.0f, 1.0f), lm::vec3(0.8f, 0.8f, 0.8f), lm::vec3(1.0f, 1.0f, 1.0f), 1.0f, 0.09f, 0.032f);

	getSceneGraph().push_back(light1);
	getSceneGraph().push_back(light2);
	getSceneGraph().push_back(light3);
	getSceneGraph().push_back(light4);

	Model* radioModel = Singleton::resources->Get<Model>("Assets/Meshes\\aperture_radio\\source\\radio_wrapped_meshfixes.fbx");
	GameObject* radio = new GameObject("Computer", radioModel, lightShader, lm::vec3(4, 2, -4.5), lm::vec3(0, 0, 0), lm::vec3(1, 1, 1));
	irrklang::ISound* music = soundEngine->play3D("Assets/Sound/BackGround.mp3", irrklang::vec3df(radio->GetPos()->X(), radio->GetPos()->Y(), radio->GetPos()->Z()), true, false, true);
	radio->CreateSoundSource(music, 1.0f, 5.0f);

	Map map;

	Model* ourModel1 = Singleton::resources->Get<Model>("Assets/Meshes\\Ultimate_Modular_Sci-Fi\\OBJ\\Props_Computer.obj");
	Model* ourModel2 = Singleton::resources->Get<Model>("Assets/Meshes\\Ultimate_Modular_Sci-Fi\\FBX\\Props_Crate.fbx");

	GameObject* structure1 = new GameObject("Computer", ourModel1, lightShader, lm::vec3(0, 2, -4.5), lm::vec3(0, 0, 0), lm::vec3(1, 1, 1));
	structure1->CreateBoxRigidBody(Vec3(0.5f, 0.5f, 0.5f), Vec3(1.f, 1.f, 1.f), 1);

	GameObject* structure2 = new GameObject("Crate", ourModel2, lightShader, lm::vec3(-10, 2, -4.5), lm::vec3(0, 0, 0), lm::vec3(1, 1, 1));

	vec3 pPos = Singleton::cam->GetPosition();
	vec3 pRot = Singleton::cam->GetViewMatrix().extractRotation(Singleton::cam->GetViewMatrix().extractScale());
	Player* player = new Player("Player", nullptr, nullptr, pPos, pRot, vec3(1.f));
	player->CreateBoxRigidBody(vec3(0.5f, 1.f, 0.5f), vec3(0.5f, 1.f, 0.5f), 20);
	Singleton::_player = player;
	RigidBody* rb = (RigidBody*)player->GetRigidBody();
	rb->GetRB()->setActivationState(DISABLE_DEACTIVATION);
	Singleton::_player = player;

	Platform platform;
	Model* companionModel = Singleton::resources->Get<Model>("Assets/Meshes\\companion_cube\\scene.gltf");
	Companion* companionCube = new Companion("Companion", ourModel2, lightShader, lm::vec3(0, 2, 6), lm::vec3(0, 0, 0), lm::vec3(1, 1, 1));
	companionCube->CreatePointLight(lm::vec3(1.0f, 1.0f, 1.0f), lm::vec3(0.8f, 0.8f, 0.8f), lm::vec3(1.0f, 1.0f, 1.0f), 1.0f, 0.09f, 0.032f);
	companionCube->CreateBoxRigidBody(lm::vec3(0.5f, 0.5f, 0.5f), lm::vec3(1.f, 1.f, 1.f), 30);

	Model* turretModel = Singleton::resources->Get<Model>("Assets/Meshes\\tank.obj");
	Turret* turret = new Turret(turretModel, "LitShader");

	getSceneGraph().push_back(radio);
	getSceneGraph().push_back(structure1);
	getSceneGraph().push_back(structure2);
	getSceneGraph().push_back(player);
	getSceneGraph().push_back(turret);
	getSceneGraph().push_back(companionCube);

	btVector3 btCamPos;
	btVector3 btCamFront;
	while (!glfwWindowShouldClose(window))
	{
		if (currentUsedSafeFile != lastUsedSafeFile)
		{
			lastUsedSafeFile = currentUsedSafeFile;
			world.GetGameObjects().clear();
			Singleton::sceneGraph.clear();
			pointLights.clear();
			ReadScene(currentUsedSafeFile.c_str());
		}
		// per-frame time logic
		// --------------------
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = (currentFrame - lastFrame) * timeScale;
		lastFrame = currentFrame * timeScale;

		//physics
		world.UpdatePhysics(deltaTime);

		// input
		// -----
		processInput(window);
		Singleton::cam->update();

		// render
		// ------
		glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		if (EditorModeActive)
			ui->PreRender();

		UpdateLights(Singleton::cam, Singleton::resources);
		UpdateScene(deltaTime);
		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		if (EditorModeActive)
		{
			//ui->Scene();
			ui->Hierarchy();
			ui->PostRender();
		}

			if (DebugDrawerActive)
			{
				btCamPos = btVector3(Singleton::cam->GetPosition().X(), Singleton::cam->GetPosition().Y(), Singleton::cam->GetPosition().Z());
				btCamFront = btVector3(Singleton::cam->GetFoward().X(), Singleton::cam->GetFoward().Y(), Singleton::cam->GetFoward().Z());
				world.GetDiscretWorld()->getDebugDrawer()->drawLine(btCamPos, btCamFront* player->GetRaycastRange(), btVector3(0, 0, 255));
				WorldPhysics::GetInstance().GetDiscretWorld()->debugDrawWorld();
			}


		glfwSwapBuffers(window);
		glfwPollEvents();
		soundEngine->setListenerPosition(irrklang::vec3df(Singleton::cam->GetPosition().X(), Singleton::cam->GetPosition().Y(), Singleton::cam->GetPosition().Z()), irrklang::vec3df(Singleton::cam->GetFoward().X(), Singleton::cam->GetFoward().Y(), Singleton::cam->GetFoward().Z()));
	}
}

void Application::processInput(GLFWwindow* window)
{
	Application* app = (Application*)glfwGetWindowUserPointer(window);

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	float deltaTime = this->GetDeltaTime();

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS && !EditorModeActive)
		Singleton::cam->ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS && !EditorModeActive)
		Singleton::cam->ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS && !EditorModeActive)
		Singleton::cam->ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS && !EditorModeActive)
		Singleton::cam->ProcessKeyboard(RIGHT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS && !EditorModeActive)
		Singleton::cam->ProcessKeyboard(SPEED, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE && !EditorModeActive)
		Singleton::cam->ProcessKeyboard(WALK, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_F1) == GLFW_PRESS)
	{
		if (!EditorButtonPressed)
		{
			EditorButtonPressed = true;
			EditorMode();
		}
	}
	else if (glfwGetKey(window, GLFW_KEY_F1) == GLFW_RELEASE)
		EditorButtonPressed = false;

	if (glfwGetKey(window, GLFW_KEY_F2) == GLFW_PRESS)
	{
		if (!SoundButtonPressed)
		{
			SoundButtonPressed = true;
			SoundPaused = !SoundPaused;
			soundEngine->setAllSoundsPaused(SoundPaused);
		}
	}
	else if (glfwGetKey(window, GLFW_KEY_F2) == GLFW_RELEASE)
		SoundButtonPressed = false;

	if (glfwGetKey(window, GLFW_KEY_F8) == GLFW_PRESS)
	{
		if (!DebugDrawerButtonPressed)
		{
			DebugDrawerButtonPressed = true;
			DebugDrawerActive = !DebugDrawerActive;
		}
	}
	else if (glfwGetKey(window, GLFW_KEY_F8) == GLFW_RELEASE)
		DebugDrawerButtonPressed = false;

	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && !EditorModeActive)
	{
		if (!JumpButtonPressed)
		{
			JumpButtonPressed = true;
			Singleton::cam->ProcessKeyboard(JUMP, deltaTime);
		}
	}

	if (glfwGetKey(window, GLFW_KEY_F3) == GLFW_PRESS)
	{
		//temp for raycast result
		Player::RayResult result;
		bool RaycastResult = Player::Raycast(Singleton::cam->GetPosition(),
			Singleton::cam->GetFoward() * Singleton::_player->GetRaycastRange(), result);
		if (RaycastResult && result._gameObject)
			printf("Raycast hit %s\n", result._gameObject->GetName().c_str());
	}

	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS && !EditorModeActive && cooldownblue > 3.f)
	{
		cooldownblue = 0;

		Player::RayResult result;
		bool RaycastResult = Player::Raycast(Singleton::cam->GetPosition(),
			Singleton::cam->GetFoward() * Singleton::_player->GetRaycastRange(), result);

		if (RaycastResult)
			Portals::ProcessKeyboard(BLUE, result._hitPoint, deltaTime);
	}

	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS && !EditorModeActive && cooldownorange > 3.f)
	{
		cooldownorange = 0;

		Player::RayResult result;
		bool RaycastResult = Player::Raycast(Singleton::cam->GetPosition(),
			Singleton::cam->GetFoward() * Singleton::_player->GetRaycastRange(), result);

		std::cout << " x : " << result._hitPoint.X() << " z : " << result._hitPoint.Z() << std::endl;
		if (RaycastResult)
			Portals::ProcessKeyboard(ORANGE, result._hitPoint, deltaTime);
	}

	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS && !EditorModeActive)
	{
		if (!GrabbRaycastButtonPressed)
		{
			Player::RayResult result;
			bool RaycastResult = Player::Raycast(Singleton::cam->GetPosition(),
				Singleton::cam->GetFoward() * Singleton::_player->GetRaycastRange(), result);

			if (Singleton::_player->GetHoldCC())
			{
				Singleton::_player->UnGrabbCompanionCube(result);
			}
			else if (Singleton::_player->InRangeOfCompanionCube(result))
			{
				Singleton::_player->GrabbCompanionCube(result);
			}
			GrabbRaycastButtonPressed = true;
		}
	}
	else if (glfwGetKey(window, GLFW_KEY_R) == GLFW_RELEASE)
		GrabbRaycastButtonPressed = false;

	cooldownblue += deltaTime;
	cooldownorange += deltaTime;
}

ResourceManager* Application::GetResourceManager()
{
	return Singleton::resources;
}

GLFWwindow* Application::GetWindow()
{
	return this->window;
}

void Application::SetCamera(Camera* cam)
{
	Singleton::cam = cam;
}

void Application::UpdateLights(Camera* cam, ResourceManager* manager)
{
	for (int i = 0; i < dirLights.size(); ++i)
	{
			dirLights[i]->UpdateLights(Singleton::resources->Get<Shader>("LitShader"), i);
			glUniform1i(glGetUniformLocation(Singleton::resources->Get<Shader>("LitShader")->GetShaderProgram(), ("nbDirLight")), dirLights.size());
	}

	for (int i = 0; i < pointLights.size(); ++i)
	{
			pointLights[i]->UpdateLights(Singleton::resources->Get<Shader>("LitShader"), i);
			glUniform1i(glGetUniformLocation(Singleton::resources->Get<Shader>("LitShader")->GetShaderProgram(), ("nbPointLight")), pointLights.size());
	}

	for (int i = 0; i < spotLights.size(); ++i)
	{
		spotLights[i]->SetPositionDirection(cam);

			spotLights[i]->UpdateLights(Singleton::resources->Get<Shader>("LitShader"), i);
			glUniform1i(glGetUniformLocation(Singleton::resources->Get<Shader>("LitShader")->GetShaderProgram(), ("nbSpotLight")), spotLights.size());
	}
}

void Application::UpdateScene(float deltaTime)
{
	for (int i = 0; i < Singleton::sceneGraph.size(); ++i)
	{
		Singleton::sceneGraph[i]->UpdateMatrices();

		if (dynamic_cast<Turret*>(Singleton::sceneGraph[i]) != nullptr)
			dynamic_cast<Turret*>(Singleton::sceneGraph[i])->Update();

		if(dynamic_cast<Player*>(Singleton::sceneGraph[i]) != nullptr)
			dynamic_cast<Player*>(Singleton::sceneGraph[i])->Update(deltaTime);

		if(dynamic_cast<Portals*>(Singleton::sceneGraph[i]) != nullptr)
			dynamic_cast<Portals*>(Singleton::sceneGraph[i])->Update(*Singleton::_player);

		if (dynamic_cast<Platform*>(Singleton::sceneGraph[i]) != nullptr)
			dynamic_cast<Platform*>(Singleton::sceneGraph[i])->Update(deltaTime);
	}
}

void Application::LoadModels()
{
	std::string path = "Assets/Meshes";
	for (const auto& entry : std::filesystem::recursive_directory_iterator(path))
	{
		std::string pathName = entry.path().string();
		if (pathName.substr(pathName.size() - 4, 4) == ".fbx" || pathName.substr(pathName.size() - 4, 4) == ".FBX" || pathName.substr(pathName.size() - 4, 4) == ".obj" || pathName.substr(pathName.size() - 5, 5) == ".gltf")
			if (pathName == "Assets/Meshes\\Quad.obj")
			{
				Singleton::resources->Create<Model>("BluePortal", Singleton::cam);
				Singleton::resources->Create<Model>("OrangePortal", Singleton::cam);
			}
			else
				Singleton::resources->Create<Model>(pathName, Singleton::cam);
	}
}

void Application::EditorMode()
{
	EditorModeActive = !EditorModeActive;
	if (EditorModeActive)
	{
		timeScale = 0;
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
	else
	{
		timeScale = 1;
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}
}

void Application::TerminateImGui()
{
	this->ui->Terminate();
	//this->soundEngine->drop();
}

float Application::GetDeltaTime()
{
	return deltaTime;
}

std::vector<DirectionalLight*>& Application::getDirLights()
{
	return this->dirLights;
}

std::vector<PointLight*>& Application::getPointLights()
{
	return this->pointLights;
}

std::vector<SpotLight*>& Application::getSpotLights()
{
	return this->spotLights;
}

std::vector<Graph*>& Application::getSceneGraph()
{
	return Singleton::sceneGraph;
}

std::vector<Shader*>& Application::getShaderList()
{
	return Singleton::shaderList;
}

Camera* Application::getCamera()
{
	return Singleton::cam;
}

bool& Application::getJumpButton()
{
	return JumpButtonPressed;
}

void Application::mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
	Application* app = (Application*)glfwGetWindowUserPointer(window);

	float xpos = static_cast<float>(xposIn);
	float ypos = static_cast<float>(yposIn);

	if (app->firstMouse)
	{
		app->lastX = xpos;
		app->lastY = ypos;
		app->firstMouse = false;
	}

	float xoffset = xpos - app->lastX;
	float yoffset = app->lastY - ypos; // reversed since y-coordinates go from bottom to top

	app->lastX = xpos;
	app->lastY = ypos;

	if(app->EditorModeActive == false)
		Singleton::cam->ProcessMouseMovement(xoffset, yoffset);
}

void Application::SaveScene()
{
	rapidjson::StringBuffer ss;
	rapidjson::PrettyWriter<rapidjson::StringBuffer> pw(ss);
	pw.StartObject();
	pw.Key("GameObjects");
	pw.StartArray();
	for (int i = 0; i < Singleton::sceneGraph.size(); i++)
	{
		if(dynamic_cast<GameObject*>(Singleton::sceneGraph[i]) != nullptr)
			dynamic_cast<GameObject*>(Singleton::sceneGraph[i])->SaveObject(pw);
	}
	pw.EndArray();
	pw.EndObject();

	std::ofstream file(currentUsedSafeFile);
	std::string s = ss.GetString();
	file << s;
	file.close();
}

void Application::SaveScene(std::string filePath)
{
	rapidjson::StringBuffer ss;
	rapidjson::PrettyWriter<rapidjson::StringBuffer> pw(ss);
	pw.StartObject();
	pw.Key("GameObjects");
	pw.StartArray();
	for (int i = 0; i < Singleton::sceneGraph.size(); i++)
	{
		if (dynamic_cast<GameObject*>(Singleton::sceneGraph[i]) != nullptr)
			dynamic_cast<GameObject*>(Singleton::sceneGraph[i])->SaveObject(pw);
	}
	pw.EndArray();
	pw.EndObject();

	std::string txt = "Scene/" + filePath + ".json";
	currentUsedSafeFile = txt;
	std::ofstream file(txt);
	std::string s = ss.GetString();
	file << s;
	file.close();
}

void Application::NewScene(std::string filePath)
{
	rapidjson::StringBuffer ss;
	rapidjson::PrettyWriter<rapidjson::StringBuffer> pw(ss);
	pw.StartObject();
	pw.Key("GameObjects");
	pw.StartArray();
	pw.EndArray();
	pw.EndObject();

	std::string txt = "Scene/" + filePath + ".json";
	currentUsedSafeFile = txt;
	std::ofstream file(txt);
	file.close();
}

bool Application::ReadScene(char const* filepath)
{
	std::fstream file(filepath);
	if (!file.good())
		return false;
	bool fileEmpty = true;
	std::string s;
	file >> s;
	std::vector<Graph*> childs;
	while (!file.eof())
	{
		file >> s;

		if (s == "\"GameObjects\":")
			fileEmpty = CreateSceneObject(file, childs);

		s.clear();
	}

	file.close();

	if (!fileEmpty)
	{
		Singleton::sceneGraph = childs;
	}

	return !fileEmpty;
}

bool Application::CreateSceneObject(std::fstream& file, std::vector<Graph*>& childs)
{
	std::vector<Graph*> newChilds;
	std::string s, num, objectName, nameShader, vertexShader, fragShader, geoShader, model;
	lm::vec3 pos, rot, scale, direction, halfExtend, rigidBodyScale, PointAmbiantColor, PointDiffuseColor, PointSpecularColor, SpotAmbiantColor, SpotDiffuseColor, SpotSpecularColor;
	float PointConstant, PointLinear, PointQuadratic, SpotConstant, SpotLinear, SpotQuadratic, cutOff, outerCutOff, mass, radius, height;
	bool pointLightOn = false, spotLightOn = false, boxOn = false, capsuleOn = false, sphereOn = false;

	while (!file.eof())
	{
		file >> s;
		if (s == "[")
			continue;
		else if (s == "[]")
			return true;

		if (s == "\"ObjectName\":")
		{
			file >> objectName;
			num.clear();
			for (int i = 1; i < objectName.size() - 2; i++)
				num += objectName[i];
			objectName = num;
		}

		if (s == "\"ShaderName\":")
		{
			file >> nameShader;
			num.clear();
			for (int i = 1; i < nameShader.size() - 2; i++)
				num += nameShader[i];
			nameShader = num;
		}

		else if (s == "\"VertexShaderPath\":")
		{
			file >> vertexShader;
			num.clear();
			for (int i = 1; i < vertexShader.size() - 2; i++)
				num += vertexShader[i];
			vertexShader = num;
		}

		else if (s == "\"FragmentShaderPath\":")\
		{
			file >> fragShader;
			num.clear();
			for (int i = 1; i < fragShader.size() - 2; i++)
				num += fragShader[i];
			fragShader = num;
		}

		else if (s == "\"GeometryShaderPath\":")
		{
			file >> geoShader;
			num.clear();
			for (int i = 1; i < geoShader.size() - 2; i++)
				num += geoShader[i];
			geoShader = num;
		}

		else if (s == "\"ModelPath\":")
		{
			file >> model;
			num.clear();
			for (int i = 1; i < model.size() - 2; i++)
			{
				if (model[i] == '\\')
					i++;
				num += model[i];
			}
			model = num;
		}

		else if (s == "\"Position\":")
		{
			file >> s;
			num.clear();
			for (int i = 1; i < s.size(); i++)
				num += s[i];
			pos.X() = std::stof(num.c_str());
			file >> s;
			pos.Y() = std::stof(s.c_str());
			file >> s;
			pos.Z() = std::stof(s.c_str());
		}

		else if (s == "\"Rotation\":")
		{
			file >> s;
			num.clear();
			for (int i = 1; i < s.size(); i++)
				num += s[i];
			rot.X() = std::stof(num.c_str());
			file >> s;
			rot.Y() = std::stof(s.c_str());
			file >> s;
			rot.Z() = std::stof(s.c_str());
		}

		else if (s == "\"Scale\":")
		{
			file >> s;
			num.clear();
			for (int i = 1; i < s.size(); i++)
				num += s[i];
			scale.X() = std::stof(num.c_str());
			file >> s;
			scale.Y() = std::stof(s.c_str());
			file >> s;
			scale.Z() = std::stof(s.c_str());
		}

		else if (s == "\"Component\":")
		{
			pointLightOn = false, spotLightOn = false, boxOn = false, capsuleOn = false, sphereOn = false;
			while (s != "]," && s != "[],")
			{
				file >> s;
				if (s == "\"Type\":")
				{
					file >> s;
					if (s == "\"PointLight\",")
						pointLightOn = CreatePointLight(file, PointAmbiantColor, PointDiffuseColor, PointSpecularColor, PointConstant, PointLinear, PointQuadratic);

					else if (s == "\"SpotLight\",")
						spotLightOn = CreateSpotLight(file, SpotAmbiantColor, SpotDiffuseColor, SpotSpecularColor, SpotConstant, SpotLinear, SpotQuadratic, direction, cutOff, outerCutOff);

					else if (s == "\"BoxRigidBody\",")
						boxOn = CreateBoxRigidBody(file, mass, halfExtend, rigidBodyScale);

					else if (s == "\"CapsuleRigidBody\",")
						capsuleOn = CreateCapsuleRigidBody(file, mass, radius, height);

					else if (s == "\"SphereRigidBody\",")
						sphereOn = CreateSphereRigidBody(file, mass, radius);
				}
			}
		}

		else if (s == "\"Childs\":")
			CreateSceneObject(file, newChilds);

		else if (s == "}," || s == "}")
		{
			GameObject* newObject;

			Model* newModel;
			if (Singleton::resources->Get<Model>(model) == nullptr)
				if (model != "Null")
					newModel = Singleton::resources->Create<Model>(model, Singleton::cam);
				else
					newModel = nullptr;
			else
				newModel = Singleton::resources->Get<Model>(model);

			Shader* newShader;
			if (Singleton::resources->Get<Shader>(nameShader) == nullptr)
				if (nameShader != "Null")
					newShader = Singleton::resources->Create<Shader>(nameShader, vertexShader.c_str(), fragShader.c_str(), geoShader.c_str());
				else
					newShader = nullptr;
			else
				newShader = Singleton::resources->Get<Shader>(nameShader);

			if (objectName == "Turret")
				newObject = new Turret(newModel, nameShader);

			else if (objectName == "Player")
			{
				Singleton::_player = new Player(objectName, newModel, newShader, pos, rot, scale);
				newObject = Singleton::_player;
			}

			else if (objectName == "Platform")
				newObject = new Platform();

			else if (objectName == "Portal")
				newObject = new Portals(objectName, pos);

			else if (objectName == "Companion")
				newObject = new Companion(objectName, newModel, newShader, pos, rot, scale);

			else
				newObject = new GameObject(objectName, newModel, newShader, pos, rot, scale);

			if (pointLightOn)
				newObject->CreatePointLight(PointAmbiantColor, PointDiffuseColor, PointSpecularColor, PointConstant, PointLinear, PointQuadratic);
			else if (spotLightOn)
				newObject->CreateSpotLight(SpotAmbiantColor, SpotDiffuseColor, SpotSpecularColor, direction, cutOff, outerCutOff, SpotConstant, SpotLinear, SpotQuadratic);
			else if (boxOn)
				newObject->CreateBoxRigidBody(halfExtend, rigidBodyScale, mass);
			else if (capsuleOn)
				newObject->CreateCapsuleRigidBody(height, radius, mass);
			else if (sphereOn)
				newObject->CreateSphereRigidBody(radius, mass);

			for (int i = 0; i < newChilds.size(); i++)
			{
				newObject->AddChild(newChilds[i], this->window);
			}
			childs.push_back(newObject);
		}

		else if (s == "]")
			break;
	}
	return false;
}

bool Application::CreatePointLight(std::fstream& file, lm::vec3& ambiantColor, lm::vec3& diffuseColor, lm::vec3& specularColor, float& constant, float& linear, float& quadratic)
{
	std::string s;
	std::string num;
	while (s != "}")
	{
		file >> s;
		if (s == "\"AmbiantColor\":")
		{
			file >> s;
			num.clear();
			for (int i = 1; i < s.size(); i++)
				num += s[i];
			ambiantColor.X() = std::stof(num.c_str());
			file >> s;
			ambiantColor.Y() = std::stof(s.c_str());
			file >> s;
			ambiantColor.Z() = std::stof(s.c_str());
		}

		else if (s == "\"DiffuseColor\":")
		{
			file >> s;
			num.clear();
			for (int i = 1; i < s.size(); i++)
				num += s[i];
			diffuseColor.X() = std::stof(num.c_str());
			file >> s;
			diffuseColor.Y() = std::stof(s.c_str());
			file >> s;
			diffuseColor.Z() = std::stof(s.c_str());
		}

		else if (s == "\"SpecularColor\":")
		{
			file >> s;
			num.clear();
			for (int i = 1; i < s.size(); i++)
				num += s[i];
			specularColor.X() = std::stof(num.c_str());
			file >> s;
			specularColor.Y() = std::stof(s.c_str());
			file >> s;
			specularColor.Z() = std::stof(s.c_str());
		}

		else if (s == "\"Constant\":")
		{
			file >> s;
			constant = std::stof(s.c_str());
		}

		else if (s == "\"Linear\":")
		{
			file >> s;
			linear = std::stof(s.c_str());
		}

		else if (s == "\"Quadratic\":")
		{
			file >> s;
			quadratic = std::stof(s.c_str());
		}
	}
	return true;
}

bool Application::CreateSpotLight(std::fstream& file, lm::vec3& ambiantColor, lm::vec3& diffuseColor, lm::vec3& specularColor, float& constant, float& linear, float& quadratic, lm::vec3& direction, float cutOff, float outerCutOff)
{
	std::string s;
	std::string num;
	while (s != "}")
	{
		file >> s;
		if (s == "\"AmbiantColor\":")
		{
			file >> s;
			num.clear();
			for (int i = 1; i < s.size(); i++)
				num += s[i];
			ambiantColor.X() = std::stof(num.c_str());
			file >> s;
			ambiantColor.Y() = std::stof(s.c_str());
			file >> s;
			ambiantColor.Z() = std::stof(s.c_str());
		}

		else if (s == "\"DiffuseColor\":")
		{
			file >> s;
			num.clear();
			for (int i = 1; i < s.size(); i++)
				num += s[i];
			diffuseColor.X() = std::stof(num.c_str());
			file >> s;
			diffuseColor.Y() = std::stof(s.c_str());
			file >> s;
			diffuseColor.Z() = std::stof(s.c_str());
		}

		else if (s == "\"SpecularColor\":")
		{
			file >> s;
			num.clear();
			for (int i = 1; i < s.size(); i++)
				num += s[i];
			specularColor.X() = std::stof(num.c_str());
			file >> s;
			specularColor.Y() = std::stof(s.c_str());
			file >> s;
			specularColor.Z() = std::stof(s.c_str());
		}

		else if (s == "\"Constant\":")
		{
			file >> s;
			constant = std::stof(s.c_str());
		}

		else if (s == "\"Linear\":")
		{
			file >> s;
			linear = std::stof(s.c_str());
		}

		else if (s == "\"Quadratic\":")
		{
			file >> s;
			quadratic = std::stof(s.c_str());
		}

		else if (s == "\"Direction\":")
		{
			file >> s;
			num.clear();
			for (int i = 1; i < s.size(); i++)
				num += s[i];
			direction.X() = std::stof(num.c_str());
			file >> s;
			direction.Y() = std::stof(s.c_str());
			file >> s;
			direction.Z() = std::stof(s.c_str());
		}

		else if (s == "\"CutOff\":")
		{
			file >> s;
			cutOff = std::stof(s.c_str());
		}

		else if (s == "\"OuterCutOff\":")
		{
			file >> s;
			outerCutOff = std::stof(s.c_str());
		}
	}
	return true;
}

bool Application::CreateBoxRigidBody(std::fstream& file, float& mass, lm::vec3& halfExtend, lm::vec3& scale)
{
	std::string s;
	std::string num;
	while (s != "}")
	{
		file >> s;
		if (s == "\"HalfExtents\":")
		{
			file >> s;
			num.clear();
			for (int i = 1; i < s.size(); i++)
				num += s[i];
			halfExtend.X() = std::stof(num.c_str());
			file >> s;
			halfExtend.Y() = std::stof(s.c_str());
			file >> s;
			halfExtend.Z() = std::stof(s.c_str());
		}

		else if (s == "\"Scale\":")
		{
			file >> s;
			num.clear();
			for (int i = 1; i < s.size(); i++)
				num += s[i];
			scale.X() = std::stof(num.c_str());
			file >> s;
			scale.Y() = std::stof(s.c_str());
			file >> s;
			scale.Z() = std::stof(s.c_str());
		}

		else if (s == "\"Mass\":")
		{
			file >> s;
			mass = std::stof(s.c_str());
		}
	}
	return true;
}

bool Application::CreateCapsuleRigidBody(std::fstream& file, float& mass, float& radius, float& height)
{
	std::string s;
	while (s != "}")
	{
		file >> s;
		if (s == "\"Mass\":")
		{
			file >> s;
			mass = std::stof(s.c_str());
		}

		else if (s == "\"Rad\":")
		{
			file >> s;
			radius = std::stof(s.c_str());
		}

		else if (s == "\"Height\":")
		{
			file >> s;
			height = std::stof(s.c_str());
		}
	}
	return true;
}

bool Application::CreateSphereRigidBody(std::fstream& file, float& mass, float& radius)
{
	std::string s;
	while (s != "}")
	{
		file >> s;
		if (s == "\"Mass\":")
		{
			file >> s;
			mass = std::stof(s.c_str());
		}

		else if (s == "\"Rad\":")
		{
			file >> s;
			radius = std::stof(s.c_str());
		}
	}
	return true;
}

void Application::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
	Application* app = (Application*)glfwGetWindowUserPointer(window);
	app->SetCamera(new Camera(90, ((float)width / height), 0.1f, 100));
	app->SCREENWIDTH = (float)width;
	app->SCREENHEIGHT = (float)height;
}