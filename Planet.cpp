#include "Planet.h"


Planet::Planet(int seed, Ogre::Vector3 position) : m_position(position), m_seed(seed), m_planetNoise(seed)
{
}


Planet::~Planet(void)
{
}

void Planet::setSeed(int seed)
{
	m_planetNoise.setSeed(seed);
	m_seed = seed;
}