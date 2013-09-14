#include "PlanetRenderer.h"
#include "Planet.h"
#include "PlanetChunk.h"

#define SQRT_3 1.732050808 //Racine carrée de trois
#define NORM SQRT_3 * m_radius

PlanetRenderer::PlanetRenderer(Planet *plnt)
{
    //TODO : Trouver le radius, le pointeur de la camera, la position puis initaliser les 6 quadtrees

    m_radius = plnt->getRadius();
    m_camera = plnt->getCamera();
    m_node = plnt->getNode();
    m_sceneManager = plnt->getSceneManager();

    //Initialization des 6 quadtrees
    m_quadtrees[0] = new PlanetChunk(Ogre::Vector3(-1/NORM, -1/NORM, -1/NORM), Ogre::Vector3(-1/NORM, -1/NORM, 1/NORM), Ogre::Vector3(-1/NORM, 1/NORM, 1/NORM), Ogre::Vector3(-1/NORM, 1/NORM, -1/NORM), m_radius, m_node, m_sceneManager);
    m_quadtrees[1] = new PlanetChunk(Ogre::Vector3(-1/NORM, -1/NORM, 1/NORM), Ogre::Vector3(1/NORM, -1/NORM, 1/NORM), Ogre::Vector3(1/NORM, 1/NORM, 1/NORM), Ogre::Vector3(-1/NORM, 1/NORM, 1/NORM), m_radius, m_node, m_sceneManager);
    m_quadtrees[2] = new PlanetChunk(Ogre::Vector3(1/NORM, -1/NORM, 1/NORM), Ogre::Vector3(1/NORM, -1/NORM, -1/NORM), Ogre::Vector3(1/NORM, 1/NORM, -1/NORM), Ogre::Vector3(1/NORM, 1/NORM, 1/NORM), m_radius, m_node, m_sceneManager);
    m_quadtrees[3] = new PlanetChunk(Ogre::Vector3(1/NORM, -1/NORM, -1/NORM), Ogre::Vector3(-1/NORM, -1/NORM, -1/NORM), Ogre::Vector3(-1/NORM, 1/NORM, -1/NORM), Ogre::Vector3(1/NORM, 1/NORM, -1/NORM), m_radius, m_node, m_sceneManager);
    m_quadtrees[4] = new PlanetChunk(Ogre::Vector3(-1/NORM, -1/NORM, -1/NORM), Ogre::Vector3(1/NORM, -1/NORM, -1/NORM), Ogre::Vector3(1/NORM, -1/NORM, 1/NORM), Ogre::Vector3(-1/NORM, -1/NORM, 1/NORM), m_radius, m_node, m_sceneManager);
    m_quadtrees[5] = new PlanetChunk(Ogre::Vector3(-1/NORM, 1/NORM, 1/NORM), Ogre::Vector3(1/NORM, 1/NORM, 1/NORM), Ogre::Vector3(1/NORM, 1/NORM, -1/NORM), Ogre::Vector3(-1/NORM, 1/NORM, -1/NORM), m_radius, m_node, m_sceneManager);
}
