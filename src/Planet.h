#pragma once

#include <OGRE/OgreCamera.h>
#include <OGRE/OgreSceneNode.h>

class PlanetNoise;
class PlanetRenderer;

class Planet
{
public:

    Planet(float planetRadius, int seed, Ogre::SceneNode *node, Ogre::Camera *cam, Ogre::SceneManager *scnMgr);
	~Planet(void);

    void update();

	void setSeed(int seed);

    inline int getSeed() { return m_seed; }
    inline float getRadius() { return m_radius; }
    inline Ogre::SceneNode *getNode() { return m_node; }
    inline Ogre::Camera *getCamera() { return m_camera; }
    Ogre::SceneManager *getSceneManager() { return m_sceneMgr; }
    PlanetNoise *noise() {return m_pnoise;}

private:

	int m_seed;
    float m_radius;
    PlanetNoise *m_pnoise;
    Ogre::SceneNode *m_node;
    Ogre::Camera *m_camera;
    Ogre::SceneManager *m_sceneMgr;

    PlanetRenderer *m_renderer;
};

