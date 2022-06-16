#pragma once
#include "../../libGL/include/Ressource/AssimpModel.h"
#include <GLFW/glfw3.h>
#include "../RapidJASON/include/rapidjson/stringbuffer.h"
#include "../RapidJASON/include/rapidjson/prettywriter.h"


class Graph
{
protected:
	lm::vec3 pos;
	lm::vec3 rot;
	lm::vec3 scale;
	lm::mat4 localTransform;
	lm::mat4 globalTransform;
	Graph* parent = nullptr;
	std::vector<Graph*> childs;
	Model* model = nullptr;
	Shader* shader = nullptr;
	std::string name;

public:
	Graph() {};
	Graph(std::string name, Model* model, Shader* shader, lm::vec3 position, lm::vec3 rotation, lm::vec3 scale);
	Graph(std::string name, Model* model, Shader* shader, lm::vec3 position, lm::vec3 rotation, lm::vec3 scale, Graph* parent, GLFWwindow* window);
	void AddChild(Graph* child, GLFWwindow* window);
	virtual void UpdateMatrices();
	void UpdateRotation(lm::vec3 const& newRotation);
	void UpdatePosition(lm::vec3 const& newPosition);
	void UpdateScale(lm::vec3 const& newScale);
	lm::mat4& GetGlobalMatrix();
	lm::mat4& GetLocalMatrix();
	Graph& GetParent();
	void SetParent(Graph* newParent) { parent = newParent; }
	lm::vec3& GetScale() { return scale; }
	lm::vec3* GetPos();
	lm::vec3& GetRot() { return rot; }
	std::string& GetName();
	std::vector<Graph*>& GetChilds() { return childs; }
	Model* GetModel() const { return model; }
	void SetModel(Model* model) { this->model = model; }
	virtual void SaveObject(rapidjson::PrettyWriter<rapidjson::StringBuffer>& buffer);
};