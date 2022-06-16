#include "../../../include/Core/DataStructure/Graph.h"
#include "../../../include/Application.h"

Graph::Graph(std::string name, Model* model, Shader* shader, lm::vec3 position, lm::vec3 rotation, lm::vec3 scale)
{
	this->name = name;
	this->model = model;
	this->shader = shader;
	this->parent = parent;
	this->parent = nullptr;
	this->localTransform = lm::mat4::createTransformMatrix(position, rotation, scale);
	this->globalTransform = this->localTransform;
	this->pos = position;
	this->rot = rotation;
	this->scale = scale;
}

Graph::Graph(std::string name, Model* model, Shader* shader, lm::vec3 position, lm::vec3 rotation, lm::vec3 scale, Graph* parent, GLFWwindow* window)
{
	this->name = name;
	this->model = model;
	this->shader = shader;
	this->parent = parent;
	this->parent->AddChild(this, window);
	this->localTransform = lm::mat4::createTransformMatrix(position, rotation, scale);
	this->globalTransform = this->parent->GetGlobalMatrix() * this->localTransform;
	this->pos = position;
	this->rot = rotation;
	this->scale = scale;
}


void Graph::AddChild(Graph* child, GLFWwindow* window)
{
	this->childs.push_back(child);
	if (&child->GetParent() != this)
		child->parent = this;

	Application* app = (Application*)glfwGetWindowUserPointer(window);
	std::vector<Graph*>& sceneGraph = app->getSceneGraph();
	for (int i = 0; i < sceneGraph.size(); ++i)
	{
		if (sceneGraph[i] == child)
		{
			sceneGraph[i] = this;
			break;
		}
	}

	bool alreadyIn = false;
	for (std::vector<Graph*>::iterator it = sceneGraph.begin(); it != sceneGraph.end(); ++it)
	{
		if ((*it)== this)
		{
			if (alreadyIn == true)
			{
				sceneGraph.erase(it);
				return;
			}
			else
				alreadyIn = true;
		}
	}
}

void Graph::UpdateMatrices()
{
	if (this->parent != nullptr)
		this->globalTransform = this->parent->GetGlobalMatrix() * this->localTransform;

	this->pos.X() = this->globalTransform[3][0];
	this->pos.Y() = this->globalTransform[3][1];
	this->pos.Z() = this->globalTransform[3][2];

	this->scale = this->globalTransform.extractScale();

	this->rot = this->globalTransform.extractRotation(this->scale);

	if (this->model != nullptr)
	{
		if (name != "Gun")
			this->model->Draw(this->globalTransform, *this->shader);
		else
		{
			lm::mat4 mat = Singleton::cam->GetProjMatrix().scale(lm::vec3(0.05f, 0.05f, 0.0f));
			this->model->Draw(mat, *this->shader, true);
		}
	}


	for (int i = 0; i < this->childs.size(); i++)
	{
		this->childs[i]->UpdateMatrices();
	}
}

void Graph::UpdateRotation(lm::vec3 const& newRotation)
{
	this->rot = newRotation;
	this->localTransform = lm::mat4::createTransformMatrix(pos, rot, scale);
	if (this->parent == nullptr)
		globalTransform = localTransform;

}

void Graph::UpdatePosition(lm::vec3 const& newPosition)
{
	this->pos = newPosition;
	this->localTransform = lm::mat4::createTransformMatrix(pos, rot, scale);
	if (this->parent == nullptr)
		globalTransform = localTransform;

}

void Graph::UpdateScale(lm::vec3 const& newScale)
{
	this->scale = newScale;
	this->localTransform = lm::mat4::createTransformMatrix(pos, rot, scale);
	if (this->parent == nullptr)
		globalTransform = localTransform;

}

lm::mat4& Graph::GetGlobalMatrix()
{
	return this->globalTransform;
}

lm::mat4& Graph::GetLocalMatrix()
{
	return this->localTransform;
}

Graph& Graph::GetParent()
{
	return *this->parent;
}

lm::vec3* Graph::GetPos()
{
	return &this->pos;
}

std::string& Graph::GetName()
{
	return this->name;
}

void Graph::SaveObject(rapidjson::PrettyWriter<rapidjson::StringBuffer>& buffer)
{
	buffer.StartObject();

	buffer.Key("ObjectName");
	buffer.String(this->name.c_str());

	buffer.Key("ShaderName");
	buffer.String(this->shader->name.c_str());

	buffer.Key("VertexShaderPath");
	buffer.String(this->shader->vertexPath.c_str());

	buffer.Key("FragmentShaderPath");
	buffer.String(this->shader->fragmentPath.c_str());

	buffer.Key("GeometryShaderPath");
	buffer.String(this->shader->geometryPath.c_str());

	buffer.Key("ModelPath");
	buffer.String(this->model->path.c_str());

	buffer.Key("Position");
	buffer.SetFormatOptions(rapidjson::PrettyFormatOptions::kFormatSingleLineArray);
	buffer.StartArray();
	buffer.Double(this->pos.X());
	buffer.Double(this->pos.Y());
	buffer.Double(this->pos.Z());
	buffer.EndArray();
	buffer.SetFormatOptions(rapidjson::PrettyFormatOptions::kFormatDefault);

	buffer.Key("Rotation");
	buffer.SetFormatOptions(rapidjson::PrettyFormatOptions::kFormatSingleLineArray);
	buffer.StartArray();
	buffer.Double(this->rot.X());
	buffer.Double(this->rot.Y());
	buffer.Double(this->rot.Z());
	buffer.EndArray();
	buffer.SetFormatOptions(rapidjson::PrettyFormatOptions::kFormatDefault);

	buffer.Key("Scale");
	buffer.SetFormatOptions(rapidjson::PrettyFormatOptions::kFormatSingleLineArray);
	buffer.StartArray();
	buffer.Double(this->scale.X());
	buffer.Double(this->scale.Y());
	buffer.Double(this->scale.Z());
	buffer.EndArray();
	buffer.SetFormatOptions(rapidjson::PrettyFormatOptions::kFormatDefault);

	buffer.Key("Childs");
	buffer.StartArray();
	for (int i = 0; i < this->childs.size(); i++)
	{
		this->childs[i]->SaveObject(buffer);
	}
	buffer.EndArray();
	buffer.EndObject();
}