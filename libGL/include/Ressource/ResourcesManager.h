#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <filesystem>
#include "../LowRenderer/Camera.h"

class IResource
{
public:
	virtual ~IResource();
};

class ResourceManager
{
	std::unordered_map<std::string, IResource*> resources;

public :

	~ResourceManager();
	template<typename T>
	T* Create(std::string const& name)
	{
		static_assert(std::is_base_of_v<IResource, T>, "The given parameter is not an allowed type to used in this function");
		T* newResource = new T(name);
		std::unordered_map<std::string, IResource*>::iterator it = this->resources.find(name);
		if (it != resources.end())
			delete(it->second);
		this->resources[name] = newResource;
		return newResource;
	}

	template<typename T, typename ...Args>
	T* Create(std::string const& name, Args... args)
	{
		T* newResource = new T(name.c_str() , args...);
		if (newResource == nullptr)
			return nullptr;
		std::unordered_map<std::string, IResource*>::iterator it = this->resources.find(name);
		if (it != resources.end())
			delete(it->second);
		this->resources[name] = newResource;
		return newResource;
	}

	template<typename T>
	T* Get(std::string const& name)
	{
		std::unordered_map<std::string, IResource*>::iterator it = this->resources.find(name);
		if (it != resources.end())
			return static_cast<T*>(it->second);
		else
			return nullptr;
	}

	void Delete(std::string const& name);
	std::unordered_map<std::string, IResource*>& getResources();
};