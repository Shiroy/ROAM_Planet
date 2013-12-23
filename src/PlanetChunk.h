#pragma once

/*
 *La planète est un cube mappé sur une sphère. Chaque face de ce cube "racine"
 *représente un quadtree. Contrairement à ROAM on ne gère plus ici chaque
 *triagnle individuellement. Chaque noeud du cube représente une partie de la
 *planète et les quatres enfants de ce noeud eprésente la même surface de la
 *planète avec une résolution deux fois plus grande : c'est le principe de
 *l'algorithme chunked LOD.
 *
 *L'avantage de ce système est de considérablement allèger la charge de travail
 *dédiée au processeur : il ne s'occupe que des 6 quadtrees et la carte graphique
 *s'occupe des meshs.
 */

/*
 *La classe PlanetChunk représente un noeud d'un quadtree.
 *La classe PlanetRenderer dispose de six noeuds de base (1 par face du cube racine).
 *Les noeuds s'occupent seuls de gérer s'il doivent augmenter ou diminuer le niveau
 *de détail de la partie de la planète qui leur est affecté.
 */

#include <OGRE/OgreManualObject.h>
#include <OGRE/OgrePlaneBoundedVolume.h>

#define VERTEX_PER_CHUNK 32 //Vertex par coté
#define MIN_LENGHT 3.5f //Carré de 1 m au lvl max de LOD
#define MAX_DEPTH_ALTITUDE 100.0f
#define MIN_DEPTH_ALTITUDE 125.0f

class Planet;

class PlanetChunk
{
public:
    PlanetChunk(Ogre::Vector3 bottomLeftCorner, Ogre::Vector3 bottomRightCorner, Ogre::Vector3 topRightCorner, Ogre::Vector3 topLefCorner, float radius, Ogre::SceneNode *node, Ogre::SceneManager *scnMgr, Ogre::Camera *cam, float error, Planet *planet, int currentDepth = 0);
    ~PlanetChunk();

    void update();

    inline bool needSplit();
    inline bool needMerge();
private:

    Ogre::Vector3 m_blC;
    Ogre::Vector3 m_brC;
    Ogre::Vector3 m_trC;
    Ogre::Vector3 m_tlC;

    Ogre::Vector3 centralPointExact; //Remove this hack

    float m_radius;
    Ogre::SceneNode *m_node;
    Ogre::SceneManager *m_sceneManager;
    Ogre::Camera *m_camera;
    Planet *m_planet;
    Ogre::Plane *m_plane; //Plan representant approximativement la surface de la section de planète
    Ogre::PlaneBoundedVolume m_boundedVolume;

    float m_edgeLenght;

    PlanetChunk *m_childs[4];
    Ogre::ManualObject *m_obj; //Représente l'object affichable par ogre

    float m_error;
    Ogre::Vector3 m_centralPoint;

    int m_maxDepth;
    int m_currentDepth;
};

