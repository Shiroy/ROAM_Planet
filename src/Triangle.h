#pragma once

#include "PlanetNoise.h"
#include "Diamond.h"
#include <OGRE/OgreVector3.h>

#define VERTEX_NOT_DEFINED 0xffffffff

class Vertex
{
public:
    Vertex(float x, float y, float z);
    Vertex(Ogre::Vector3 vec);

    int getIndex(int &lastIndex, Ogre::ManualObject *obj);
    bool hasIndex() {return m_index != VERTEX_NOT_DEFINED;}
    void reinitIndex() {m_index = VERTEX_NOT_DEFINED;}

    Ogre::Vector3 getCoord() {return Ogre::Vector3(m_x, m_y, m_z);}

    float m_x, m_y, m_z;

private:
    float m_r, m_g, m_b;
    unsigned int m_index;
};

namespace Ogre
{
	class ManualObject;
}

class Sphere;

class Triangle
{
	friend class Diamond;
public:
    Triangle(Vertex *v1, Vertex *v2, Vertex *v3, Triangle *parent, int recurseLevel, std::vector<Diamond*> *diamondList, Sphere *plnt);
    Triangle(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3, float normal1, float normal2, float normal3, Triangle *parent, int recurseLevel, std::vector<Diamond*> *diamondList, Sphere *plnt); //SHOULD NOT BE USED OUT OF THE SPHERE CONSTRUCTOR !!!
	~Triangle(void);

	void calculateNormal();

	void setVoisin(int voisin, Triangle *v);
	Triangle *getVoisin(int voisin);

	void setEnfant(int enfant, Triangle *e);
	Triangle* getEnfant(int enfant);

	Triangle* getParent();

    int render(Ogre::ManualObject *obj, int &nbTri, int nbRecurse, int &lastIndex);

	void split(float radius, PlanetNoise *pnoise);
	void splitIfNeeded(Ogre::Vector3 dPos, float radius, bool &meshUpdated, Ogre::Camera *m_cam, PlanetNoise *pnoise);
    bool needsSplit(Ogre::Vector3 dPos, Ogre::Camera *m_cam);
	void merge();

	float ratio(Ogre::Camera *m_cam);

	float error(Ogre::Vector3 dPos, Ogre::Camera *m_cam, float radius);

	static bool belongToADiamond(Triangle *t);

	bool isVisible(Ogre::Camera *m_cam);

	int getRecurseLevel() {return m_recurseLevel;}

	static Ogre::Vector2 getScreenCoordinate(Ogre::Vector3, Ogre::Camera *m_cam);

    Sphere *getPlanet() {return m_planet;}

private:

    Vertex *v[3];

	Ogre::Vector3 normal;

	Triangle *parent;
	Triangle *voisin[3];
	Triangle *enfant[2];

    Sphere *m_planet;

	Diamond *diamond;
	std::vector<Diamond*> *diamondList;

	int m_recurseLevel;
};

