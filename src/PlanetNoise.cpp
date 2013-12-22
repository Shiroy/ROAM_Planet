#include "PlanetNoise.h"
#include "simplexnoise.h"
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iostream>


PlanetNoise::PlanetNoise(int seed)
{
    //setSeed(seed);
}

PlanetNoise::~PlanetNoise(void)
{
}

float PlanetNoise::noise(float x, float y, float z, float planetRadius)
{
    #define PRE_SCALAR 5.0f

    /*x = (x + planetRadius)/planetRadius;
    y = (y + planetRadius)/planetRadius;
    z = (z + planetRadius)/planetRadius;*/
    /*float noise, noise2 = 0;

	noise = scaled_raw_noise_3d(0, 1, x, y, z, m_perm);
	noise2 = pow(scaled_raw_noise_3d(0, 1, x*2+1034.f, y*2+837.f, z*2+103.f, m_perm),15);

	//std::cout << "Noise : " << noise << std::endl;

	return 0.f;
    return -500.f + ( noise * 700.f) + noise2*5000.f;*/

    float noise = scaled_octave_noise_3d(8, 0.4f, 3.0f, -30000.0f, 40000.0f, x*PRE_SCALAR, y*PRE_SCALAR, z*PRE_SCALAR);
    //std::cout << "Noise : " << noise << std::endl;
    return noise;
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



	// Reset rand seed
	srand(time(NULL));
}
