#include "..\include\UIManager.h"
#include <imgui_demo.cpp>
#include <LibMaths.h>
#include "../include/Bullet/CustomMotionState.h"
#include "../include/Bullet/RigidBody.h"

Graph gr;

UIManager::UIManager(GLFWwindow* window, bool& EditorModeActive) : showGameObjects(EditorModeActive)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");
}

void UIManager::PreRender()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	ImGuiStyle& style = ImGui::GetStyle();
	style.Colors[ImGuiCol_WindowBg] = ImColor(30, 30, 30, 1);
}

void UIManager::Scene()
{
	ImGui::Begin("Scene");

	ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
	lm::vec2 mSize (viewportPanelSize.x, viewportPanelSize.y);

	ImGui::End();
}

void UIManager::Hierarchy()
{
	ImGui::Begin("Editor Window", &showGameObjects, ImGuiWindowFlags_MenuBar);
	ImGui::PushItemWidth(ImGui::GetFontSize() * -12);
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("Menu"))
		{
			ShowExampleMenuFile();
			ImGui::EndMenu();
		}

		ImGui::EndMenuBar();
	}

	//show a list of game objects
	ImGui::LabelText("", "Game Objects in Scene.");

	if (ImGui::TreeNode("Object List"))
	{
		ImGui::PushStyleVar(ImGuiStyleVar_IndentSpacing, ImGui::GetFontSize() * 3);
		for (int i = 0; i < Singleton::sceneGraph.size(); i++)
		{
			ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;
			if (Singleton::sceneGraph[i]->GetChilds().size() == 0)
			{
				ImGuiTreeNodeFlags leaf_node_flags = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen; // ImGuiTreeNodeFlags_Bullet
				ImGui::TreeNodeEx((void*)(intptr_t)i, selectedObject == Singleton::sceneGraph[i] ? leaf_node_flags | ImGuiTreeNodeFlags_Selected : leaf_node_flags, Singleton::sceneGraph[i]->GetName().c_str());
				if (ImGui::IsItemClicked())
					selectedObject = dynamic_cast<GameObject*>(Singleton::sceneGraph[i]);
			}
			else
			{
				bool node_open = ImGui::TreeNodeEx((void*)(intptr_t)i, node_flags, Singleton::sceneGraph[i]->GetName().c_str());
				treeChildren(node_flags, node_open, i);
			}
		}
		ImGui::TreePop();
		ImGui::PopStyleVar();
	}

	ImGui::End();

	if (SavingScene) ShowSaveScreen(&SavingScene);
	if (CreatingScene) ShowCreateScreen(&CreatingScene);
	if (selectedObject != nullptr)
	{
		bool windowOpened = true;
		std::string txt = selectedObject->GetName() + "\'s Component";
		ImGui::Begin(txt.c_str(), &windowOpened);
		EditObject(selectedObject);
		ImGui::End();
		if (!windowOpened)
			selectedObject = nullptr;
	}
}

void UIManager::PostRender()
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void UIManager::EditObject(GameObject* gameObject)
{
	//float xPosition = gameObject->GetPos()->X();
	//float yPosition = gameObject->GetPos()->Y();
	//float zPosition = gameObject->GetPos()->Z();

	////ImGui::Begin(gameObjectName);
	//ImGui::Text("X Position");
	//ImGui::SliderFloat("##X", &xPosition, -50, 50);
	//ImGui::Text("Y Position");
	//ImGui::SliderFloat("##Y", &yPosition, -50, 50);
	//ImGui::Text("Z Position");
	//ImGui::SliderFloat("##Z", &zPosition, -50, 50);

	/*std::cout << "X Position" << xPosition << std::endl;
	std::cout << "X Position" << yPosition << std::endl;
	std::cout << "X Position" << zPosition << std::endl;

	float xRotation = gameObject->GetRot().X();
	std::cout << "X Rotation" << xRotation << std::endl;
	float yRotation = gameObject->GetRot().Y();
	std::cout << "Y Rotation" << yRotation << std::endl;
	float zRotation = gameObject->GetRot().Z();
	std::cout << "Z Rotation" << zRotation << std::endl;*/

	lm::vec3& rot = gameObject->GetRot();
	lm::vec3* pos = gameObject->GetPos();
	lm::vec3& scale = gameObject->GetScale();

	//ImGui::Begin(GetName());
	ImGui::Text("X Rotation");
	bool xRotChanged = ImGui::SliderFloat("##RX", &rot.X(), -180, 180);
	ImGui::Text("Y Rotation");
	bool yRotChanged = ImGui::SliderFloat("##RY", &rot.Y(), -180, 180);
	ImGui::Text("Z Rotation");
	bool zRotChanged = ImGui::SliderFloat("##RZ", &rot.Z(), -180, 180);

	ImGui::Text("X Position");
	bool xPosChanged = ImGui::SliderFloat("##PX", &pos->X(), -50, 50);
	ImGui::Text("Y Position");
	bool yPosChanged = ImGui::SliderFloat("##PY", &pos->Y(), -50, 50);
	ImGui::Text("Z Position");
	bool zPosChanged = ImGui::SliderFloat("##PZ", &pos->Z(), -50, 50);

	ImGui::Text("X Scale");
	bool xScaleChanged = ImGui::SliderFloat("##SX", &scale.X(), -50, 50);
	ImGui::Text("Y Scale");
	bool yScaleChanged = ImGui::SliderFloat("##SY", &scale.Y(), -50, 50);
	ImGui::Text("Z Scale");
	bool zScaleChanged = ImGui::SliderFloat("##SZ", &scale.Z(), -50, 50);

	RigidBody* rb = (RigidBody*)gameObject->GetRigidBody();

	if (rb != nullptr)
	{
		Bullet::CustomMotionState* cust = rb->GetMS();

		btVector3 position(pos->X(), pos->Y(), pos->Z());
		btQuaternion orientation(rot.X() * float(M_PI / 360), rot.Y() * float(M_PI / 360), rot.Z() * float(M_PI / 360));

		btTransform initialTransform;

		initialTransform.setOrigin(position);
		initialTransform.setRotation(orientation);

		rb->GetRB()->setWorldTransform(initialTransform);
		cust->setWorldTransform(initialTransform);
	}
	else
	{
		if (xRotChanged || yRotChanged || zRotChanged)
		{
			gameObject->UpdateRotation(rot);
		}

		if (xPosChanged || yPosChanged || zPosChanged)
		{
			gameObject->UpdatePosition(*pos);
		}

		if (xScaleChanged || yScaleChanged || zScaleChanged)
		{
			gameObject->UpdateScale(scale);
		}
	}
	////ImGui::Begin(gameObjectName);
	//ImGui::SliderFloat("X Rotation", &xRotation, -360, 360);
	//ImGui::SliderFloat("Y Rotation", &yRotation, -360, 360);
	//ImGui::SliderFloat("Z Rotation", &zRotation, -360, 360);
	//gameObject->GetGlobalMatrix() = gameObject->GetGlobalMatrix().zRotation(zRotation) * gameObject->GetGlobalMatrix().xRotation(xRotation) * gameObject->GetGlobalMatrix().yRotation(yRotation);
	
	//RigidBody* gameObjectRigidBody = (RigidBody*)gameObject->GetRigidBody();

	//if (gameObjectRigidBody != nullptr)
	//{
	//	Bullet::CustomMotionState* cust = gameObjectRigidBody->GetMS();

	//	btVector3 position(xPosition, yPosition, zPosition);
	//	btQuaternion orientation(xRotation /* float(M_PI / 360)*/, yRotation /* float(M_PI / 360)*/, zRotation /* float(M_PI / 360)*/);

	//	btTransform initialTransform;

	//	initialTransform.setOrigin(position);
	//	initialTransform.setRotation(orientation);

	//	gameObjectRigidBody->GetRB()->setWorldTransform(initialTransform);
	//	cust->setWorldTransform(initialTransform);
	//}
	//else
	//	gameObject->GetGlobalMatrix().createTransformMatrix(lm::vec3(xPosition, yPosition, zPosition), lm::vec3(xRotation, yRotation, zRotation), lm::vec3(1));
}

void UIManager::Terminate()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void UIManager::treeChildren(ImGuiTreeNodeFlags node_flags, bool isOpen, int index)
{
	if (isOpen)
	{
		for (int i = 0; i < Singleton::sceneGraph[index]->GetChilds().size(); i++)
		{
			if (Singleton::sceneGraph[index]->GetChilds()[i]->GetChilds().size() == 0)
			{
				ImGuiTreeNodeFlags leaf_node_flags = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen; // ImGuiTreeNodeFlags_Bullet
				ImGui::TreeNodeEx((void*)(intptr_t)i, selectedObject == Singleton::sceneGraph[index]->GetChilds()[i] ? leaf_node_flags | ImGuiTreeNodeFlags_Selected : leaf_node_flags, Singleton::sceneGraph[index]->GetChilds()[i]->GetName().c_str());
				if (ImGui::IsItemClicked())
					selectedObject = dynamic_cast<GameObject*>(Singleton::sceneGraph[index]->GetChilds()[i]);
			}
			else
			{
				bool o = ImGui::TreeNodeEx((void*)(intptr_t)i, node_flags, Singleton::sceneGraph[index]->GetChilds()[i]->GetName().c_str());
				ImGui::TreePop();
				treeChildren(node_flags, o, i);
			}
		}
		ImGui::TreePop();
	}
}