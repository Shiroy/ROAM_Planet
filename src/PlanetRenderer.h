#pragma once

#include <OGRE/OgreCamera.h>
#include <OGRE/OgreSceneNode.h>

class Planet;
class PlanetNoise;
class PlanetChunk;


class PlanetRenderer
{
public:
    PlanetRenderer(Planet *plnt);
    ~PlanetRenderer();

    void update();

private:
    PlanetChunk *m_quadtrees[6];

    Ogre::Camera *m_camera;
    Ogre::SceneNode *m_node;
    Ogre::SceneManager *m_sceneManager;
    float m_radius;
};
