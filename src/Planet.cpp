#include "Planet.h"
#include "PlanetNoise.h"
#include "PlanetRenderer.h"

Planet::Planet(float planetRadius, int seed, Ogre::SceneNode *node, Ogre::Camera *cam, Ogre::SceneManager *scnMgr) : m_seed(seed), m_radius(planetRadius)
{
    m_pnoise = new PlanetNoise(m_seed);
    m_camera = cam;
    m_node = node;
    m_sceneMgr = scnMgr;
    m_renderer = new PlanetRenderer(this);
}


Planet::~Planet(void)
{
	delete m_pnoise;
}

void Planet::setSeed(int seed)
{
	m_pnoise->setSeed(seed);
	m_seed = seed;
}
