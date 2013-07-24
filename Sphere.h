#pragma once
class Triangle;
#include <OGRE\OgreVector3.h>
#include <OGRE\OgreManualObject.h>
#include <list>
class Diamond;

namespace Ogre
{
	class ManualObject;
};

#define NULL 0

class Sphere
{
public:
	Sphere(float radius, Ogre::ManualObject *obj);
	~Sphere(void);

	void updateMesh(Ogre::Vector3 dPos, Ogre::Camera *m_cam); //dPos vecteur position entre le cente de la camera et le mesh
	void render(Ogre::ManualObject *obj, int &nbTri, int &recurseLevel);

	Triangle* getRootTriangle(int tri) {return rootTriangle[tri];}

private:
	Triangle *rootTriangle[12];
	float m_radius;
	Ogre::ManualObject *m_obj;
	std::vector<Diamond*> m_diamondList;
};

