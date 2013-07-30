#pragma once

class PlanetNoise
{
public:
	PlanetNoise(int seed);
	~PlanetNoise(void);

	float noise(float x, float y, float z);
	float operator()(float x, float y, float z) { return noise(x, y, z); }

	void setSeed(int seed);
	int getSeed() { return m_seed; }

private:
	int m_seed;
	int m_perm[512];

};

