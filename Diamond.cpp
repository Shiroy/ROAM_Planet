#include "Diamond.h"
#include "Triangle.h"
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
	pTriComposed[0]->diamond = NULL;
	pTriComposed[1]->diamond = NULL;
	pTriComposed[2]->diamond = NULL;
	pTriComposed[3]->diamond = NULL;
}

bool Diamond::canBeMerged(Ogre::Camera *m_cam)
{
	if(removed)
		return false;

	if(pTriComposed[0]->parent == NULL) //Root triangle
		return false;

	if(!isDiamondVisible(m_cam))
		return true;

	return false;
}

bool Diamond::isDiamondVisible(Ogre::Camera *m_cam)
{
	if(!pTriComposed[0]->isVisible(m_cam) && !pTriComposed[1]->isVisible(m_cam) && !pTriComposed[2]->isVisible(m_cam) && !pTriComposed[3]->isVisible(m_cam))
		return false;
	else
		return true;
}