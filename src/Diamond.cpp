#include "Diamond.h"
#include "Triangle.h"
#include <OGRE/OgreCamera.h>
#include <OGRE/OgreVector2.h>
#define NULL 0

Diamond::Diamond(Triangle *t1, Triangle *t2, Triangle *t3, Triangle *t4)
{
	pTriComposed[0] = t1;
	pTriComposed[1] = t2;
	pTriComposed[2] = t3;
	pTriComposed[3] = t4;

	pTriComposed[0]->diamond = this;
	pTriComposed[1]->diamond = this;
	pTriComposed[2]->diamond = this;
	pTriComposed[3]->diamond = this;

	removed = false;
}

Diamond::Diamond(Triangle *t)
{
	pTriComposed[0] = t;
	pTriComposed[1] = t->voisin[0];
	pTriComposed[2] = t->voisin[0]->voisin[0];
	pTriComposed[3] = t->voisin[0]->voisin[0]->voisin[0];

	pTriComposed[0]->diamond = this;
	pTriComposed[1]->diamond = this;
	pTriComposed[2]->diamond = this;
	pTriComposed[3]->diamond = this;

	removed = false;
}

Diamond::~Diamond(void)
{
	/*pTriComposed[0]->diamond = NULL;
	pTriComposed[1]->diamond = NULL;
	pTriComposed[2]->diamond = NULL;
	pTriComposed[3]->diamond = NULL;*/
}

bool Diamond::canBeMerged(Ogre::Camera *m_cam)
{
	if(removed)
		return false;

	/*if(pTriComposed[0]->m_recurseLevel <= 9)
		return false;*/

	if(pTriComposed[0]->parent == NULL) //Root triangle
		return false;

	/*if(!isDiamondVisible(m_cam))
		return true;*/

	Vertex center = pTriComposed[0]->v[1];
		Ogre::Vector3 milieu;
		milieu.x = center.x;
		milieu.y = center.y;
		milieu.z = center.z;
		Ogre::Vector3 distance = milieu - m_cam->getPosition();

		Ogre::Vector3 edge;
		edge.x = pTriComposed[0]->v[2].x - pTriComposed[0]->v[0].x;
		edge.y = pTriComposed[0]->v[2].y - pTriComposed[0]->v[0].y;
		edge.z = pTriComposed[0]->v[2].z - pTriComposed[0]->v[0].z;


		Ogre::Vector3 v1(pTriComposed[0]->v[0].x - pTriComposed[0]->v[1].x, pTriComposed[0]->v[0].y - pTriComposed[0]->v[1].y, pTriComposed[0]->v[0].z - pTriComposed[0]->v[1].z);
		Ogre::Vector3 v2(pTriComposed[0]->v[0].x - pTriComposed[0]->v[2].x, pTriComposed[0]->v[0].y - pTriComposed[0]->v[2].y, pTriComposed[0]->v[0].z - pTriComposed[0]->v[2].z);
		Ogre::Vector3 normal = v1.crossProduct(v2);
		normal.normalise();

		Ogre::Vector3 camPos = m_cam->getPosition();
		camPos.normalise();
		float val  = camPos.dotProduct(normal);

		if(val <= 0) // le triangle est cache car il est sur l'autre face de la planete
			return true;
		
        edge *= (std::pow(val, 2)+1/std::pow(val+1,2));
		return (edge.squaredLength()/distance.squaredLength()*15) < 1;

	/*Ogre::Vector2 vTri2 = Triangle::getScreenCoordinate(Ogre::Vector3(pTriComposed[0]->v[2].x, pTriComposed[0]->v[2].y, pTriComposed[0]->v[2].z), m_cam), vTri0 = Triangle::getScreenCoordinate(Ogre::Vector3(pTriComposed[0]->v[0].x, pTriComposed[0]->v[0].y, pTriComposed[0]->v[0].z), m_cam);
	return (vTri2 - vTri0).length() < 200;*/

	return false;
}

bool Diamond::isDiamondVisible(Ogre::Camera *m_cam)
{
	if(!pTriComposed[0]->isVisible(m_cam) && !pTriComposed[1]->isVisible(m_cam) && !pTriComposed[2]->isVisible(m_cam) && !pTriComposed[3]->isVisible(m_cam))
		return false;
	else
		return true;
}

void Diamond::nullAllTriangle()
{
	for(int i = 0 ; i < 4 ; i++)
		pTriComposed[i]->diamond = NULL;
}
