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
	Triangle(Vertex v1, Vertex v2, Vertex v3, Triangle *parent);
	Triangle(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3, float normal1, float normal2, float normal3, Triangle *parent);
	~Triangle(void);

	void setVoisin(int voisin, Triangle *v);
	Triangle *getVoisin(int voisin);

	void setEnfant(int enfant, Triangle *e);
	Triangle* getEnfant(int enfant);

	Triangle* getParent();

	void render(Ogre::ManualObject *obj, int &nbTri, int &nbRecurse);

	void split(float radius);
	void splitIfNeeded(Ogre::Vector3 dPos, float radius, bool &meshUpdated, Ogre::Camera *m_cam);

	void mergeIfNeeded(Ogre::Vector3 dPos, float radius, bool &meshUpdated, Ogre::Camera *m_cam);
	void merge();

	float variance(float exactRadius);

private:	

	Vertex v[3];

	Triangle *parent;
	Triangle *voisin[3];
	Triangle *enfant[2];

	//Diamond *diamond;
};

