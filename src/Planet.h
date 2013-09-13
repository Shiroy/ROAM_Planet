#pragma once

#include <OGRE/Ogre.h>
#include "PlanetNoise.h"


class Planet
{
public:

	Planet(int seed, Ogre::ManualObject *obj, Ogre::Camera *cam, Ogre::SceneNode *node);
	~Planet(void);

    void renderIfUpdated() { }

	void setSeed(int seed);
	int getSeed() { return m_seed; }

private:

	int m_seed;
    PlanetNoise *m_pnoise;
};

