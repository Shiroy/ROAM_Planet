#include "Planet.h"


Planet::Planet(int seed, Ogre::ManualObject *obj, Ogre::Camera *cam, Ogre::SceneNode *node) : m_seed(seed)
{
	m_pnoise = new PlanetNoise(seed);
	m_sphere = new Sphere(1000000.0f, obj, cam, node, m_pnoise);
}


Planet::~Planet(void)
{
	delete m_sphere;
	delete m_pnoise;
}

void Planet::setSeed(int seed)
{
	m_pnoise->setSeed(seed);
	m_seed = seed;
}