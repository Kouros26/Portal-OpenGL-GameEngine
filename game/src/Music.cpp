#include "..\include\Music.h"

Music::Music(irrklang::ISound* sound, float minRange, float maxRange)
{
	this->name = "Music";
	this->sound = sound;
	this->minRange = minRange;
	this->maxRange = maxRange;
	sound->setMinDistance(this->minRange);
	sound->setMaxDistance(this->maxRange);
}

void Music::Update(lm::vec3 pos)
{
	sound->setPosition(irrklang::vec3df(pos.X(), pos.Y(), pos.Z()));
}
