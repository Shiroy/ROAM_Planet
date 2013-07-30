#include "PlanetNoise.h"
#include <cstdlib>


PlanetNoise::PlanetNoise(int seed)
{
	setSeed(seed);
}

PlanetNoise::~PlanetNoise(void)
{
}

float PlanetNoise::noise(float x, float y, float z)
{
	// Remember tu use the surcharged simplex noise function with custom_perm
	return 0.f;
}

void PlanetNoise::setSeed(int seed)
{
	srand(seed);

	// Initialize permutations tab and shuffle it
	for (int i(0); i<256; ++i)
	{
		m_perm[i] = i;
		m_perm[i+256] = i;
	}

	int r, t;
	for (int i(255); i>0; --i)
	{
		r = rand() % i;
		t = m_perm[r];

		m_perm[r] = m_perm[i];
		m_perm[r+256] = m_perm[i+256];
		m_perm[i] = t;
		m_perm[i+256] = t;
	}

	// Initialize here some other random variables that you need
}