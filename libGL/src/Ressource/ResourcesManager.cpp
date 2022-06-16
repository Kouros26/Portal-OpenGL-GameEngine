#include "../../include/Ressource/ResourcesManager.h"
#include "../../include/LowRenderer/DirectionalLight.h"
#include "../../include/LowRenderer/PointLight.h"
#include "../../include/LowRenderer/SpotLight.h"

ResourceManager::~ResourceManager()
{
	for (std::unordered_map<std::string, IResource*>::iterator it = this->resources.begin(); it != this->resources.end(); ++it)
	{
		delete(it->second);
	}
	resources.clear();
}

void ResourceManager::Delete(std::string const& name)
{
	std::unordered_map<std::string, IResource*>::iterator it = this->resources.find(name);
	if (it != resources.end())
	{
		delete(it->second);
		resources.erase(it);
	}
}

std::unordered_map<std::string, IResource*>& ResourceManager::getResources()
{
	return resources;
}

IResource::~IResource()
{
}
