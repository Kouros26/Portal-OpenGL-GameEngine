#pragma once
#include <string>

class Component
{
protected:
	std::string name;
	Component() {};
	virtual ~Component() {};
public:
	std::string& GetComponentName() { return this->name; }
};