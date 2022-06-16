#pragma once
#include "../ImGUI/imgui.h"
#include "../ImGUI/backends/imgui_impl_glfw.h"
#include "../ImGUI/backends/imgui_impl_opengl3.h"
#include "../include/Singleton.h"

class UIManager
{
private:
	bool& showGameObjects;
	GameObject* selectedObject = nullptr;
public:
	UIManager(GLFWwindow* window, bool& EditorModeActive);
	void PreRender();
	void Scene();
	void Hierarchy();
	void PostRender();
	void treeChildren(ImGuiTreeNodeFlags node_flags, bool isOpen, int index);
	void EditObject(GameObject* gameObject);
	void Terminate();
	string gameObjectName;
	std::list<Graph*>::iterator it;
};