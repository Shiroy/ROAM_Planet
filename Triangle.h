#pragma once

#include "Diamond.h"
#include <OGRE\OgreVector3.h>

struct Vertex
{
	float x, y, z;
	float nx, ny, nz;
};

namespace Ogre
{
	class ManualObject;
};

class Triangle
{
	friend class Diamond;
public:
	Triangle(Vertex v1, Vertex v2, Vertex v3, Triangle *parent, int recurseLevel, std::vector<Diamond*> *diamondList);
	Triangle(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3, float normal1, float normal2, float normal3, Triangle *parent, int recurseLevel, std::vector<Diamond*> *diamondList);
	~Triangle(void);

	void calculateNormal();

	void setVoisin(int voisin, Triangle *v);
	Triangle *getVoisin(int voisin);

	void setEnfant(int enfant, Triangle *e);
	Triangle* getEnfant(int enfant);

	Triangle* getParent();

	int render(Ogre::ManualObject *obj, int &nbTri, int nbRecurse);

	void split(float radius);
	void splitIfNeeded(Ogre::Vector3 dPos, float radius, bool &meshUpdated, Ogre::Camera *m_cam);
	bool needsSplit(Ogre::Vector3 dPos, bool &meshUpdated, Ogre::Camera *m_cam);
	void merge();

	float ratio(Ogre::Camera *m_cam);

	float error(Ogre::Vector3 dPos, Ogre::Camera *m_cam, float radius);

	static bool belongToADiamond(Triangle *t);

	bool isVisible(Ogre::Camera *m_cam);

	int getRecurseLevel() {return m_recurseLevel;}

private:	

	Vertex v[3];

	Ogre::Vector3 normal;

	Triangle *parent;
	Triangle *voisin[3];
	Triangle *enfant[2];

	Diamond *diamond;
	std::vector<Diamond*> *diamondList;

	int m_recurseLevel;
};

