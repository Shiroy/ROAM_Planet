#pragma once
class Triangle;
#include <OGRE\OgreVector3.h>
#include <OGRE\OgreManualObject.h>
#include <OGRE\OgreSceneNode.h>
#include <list>
#include "pthread\include\pthread.h"
class Diamond;

namespace Ogre
{
	class ManualObject;
};

#define NULL 0

class Sphere
{
public:
	Sphere(float radius, Ogre::ManualObject *obj, Ogre::Camera *cam, Ogre::SceneNode *node);
	~Sphere(void);

	bool updateMesh(Ogre::Vector3 dPos, Ogre::Camera *m_cam); //dPos vecteur position entre le cente de la camera et le mesh
	void render(Ogre::ManualObject *obj, int &nbTri, int &recurseLevel);
	void renderIfUpdated();

	Triangle* getRootTriangle(int tri) {return rootTriangle[tri];}

	static void *updateThread(void *arg);

	bool m_meshUpdated;
	pthread_mutex_t m_mutex;
	Ogre::ManualObject *m_obj;
	Ogre::SceneNode *m_node;

private:
	Triangle *rootTriangle[12];
	float m_radius;
	std::vector<Diamond*> m_diamondList;
	bool firstDraw;	
	pthread_t m_thread;
};

struct updateThreadArg
{
	Sphere *thisPointer;
	Ogre::Camera *m_cam;
};