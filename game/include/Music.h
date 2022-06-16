#pragma once
#include "../include/Component.h"
#include <irrKlang.h>
#include <Vec3/Vec3.h>

class Music : public Component
{
private :
	irrklang::ISound* sound = nullptr;
	float minRange;
	float maxRange;

public :
	Music(irrklang::ISound* sound, float minRange, float maxRange);
	void Update(lm::vec3 pos);
};