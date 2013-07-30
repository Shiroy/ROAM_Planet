#pragma once

#include <OGRE\Ogre.h>
#include "PlanetNoise.h"


class Planet
{
public:

	Planet(int seed, Ogre::Vector3 position);
	~Planet(void);


	void setSeed(int seed);
	int getSeed() { return m_seed; }

private:

	Ogre::Vector3 m_position;
	int m_seed;
	PlanetNoise m_planetNoise;
};

