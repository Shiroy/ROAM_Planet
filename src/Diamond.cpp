#include "Diamond.h"
#include "Triangle.h"
#include "Sphere.h"
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
    Ogre::Vector3 milieu;
    milieu.x = (pTriComposed[0]->v[2]->m_x + pTriComposed[0]->v[0]->m_x) / 2;
    milieu.y = (pTriComposed[0]->v[2]->m_y + pTriComposed[0]->v[0]->m_y) / 2;
    milieu.z = (pTriComposed[0]->v[2]->m_z + pTriComposed[0]->v[0]->m_z) / 2;
    Ogre::Vector3 distance = milieu - m_cam->getPosition();

    Ogre::Vector3 edge;
    edge.x = pTriComposed[0]->v[2]->m_x - pTriComposed[0]->v[0]->m_x;
    edge.y = pTriComposed[0]->v[2]->m_y - pTriComposed[0]->v[0]->m_y;
    edge.z = pTriComposed[0]->v[2]->m_z - pTriComposed[0]->v[0]->m_z;
    if (edge.squaredLength() < 0.25)  return true;


    Ogre::Vector3 v1(pTriComposed[0]->v[0]->m_x - pTriComposed[0]->v[1]->m_x, pTriComposed[0]->v[0]->m_y - pTriComposed[0]->v[1]->m_y, pTriComposed[0]->v[0]->m_z - pTriComposed[0]->v[1]->m_z);
    Ogre::Vector3 v2(pTriComposed[0]->v[0]->m_x - pTriComposed[0]->v[2]->m_x, pTriComposed[0]->v[0]->m_y - pTriComposed[0]->v[2]->m_y, pTriComposed[0]->v[0]->m_z - pTriComposed[0]->v[2]->m_z);
    Ogre::Vector3 normal = v1.crossProduct(v2);
    normal.normalise();

    Ogre::Vector3 camPos = m_cam->getPosition();
    camPos.normalise();
    float val  = camPos.dotProduct(normal);

    if(val <= 0) // le triangle est cache car il est sur l'autre face de la planete
        return true;

    float realRadius = pTriComposed[0]->getPlanet()->getRadius() + pTriComposed[0]->getPlanet()->getNoise()->noise(milieu.x, milieu.y, milieu.z, pTriComposed[0]->getPlanet()->getRadius());

    float variance = fabs(realRadius - milieu.length());
    float error = variance / distance.length();
    //std::cout << "Variance : " << error << std::endl;
    return (error < 0.0025f) && (distance.length() > 750);
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
