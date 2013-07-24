#include "Sphere.h"
#include "Triangle.h"
#include <math.h>

#define NORM Ogre::Math::Sqrt(3) * m_radius

Sphere::Sphere(float radius, Ogre::ManualObject *obj) : m_radius(radius)
{	
	m_obj = obj;
	rootTriangle[0] = new Triangle(-1/NORM, -1/NORM, -1/NORM, 1/NORM, -1/NORM, -1/NORM, 1/NORM, -1/NORM, 1/NORM, 0, -1, 0, NULL); //DCB (dessous)
	rootTriangle[1] = new Triangle(1/NORM, -1/NORM, 1/NORM, -1/NORM, -1/NORM, 1/NORM, -1/NORM, -1/NORM, -1/NORM, 0, -1, 0, NULL); //BAD

	rootTriangle[2] = new Triangle(-1/NORM, -1/NORM, 1/NORM, 1/NORM, -1/NORM, 1/NORM, 1/NORM, 1/NORM, 1/NORM, 0, 0, 1, NULL); //ABF (face avant)
	rootTriangle[3] = new Triangle(1/NORM, 1/NORM, 1/NORM, -1/NORM, 1/NORM, 1/NORM, -1/NORM, -1/NORM, 1/NORM, 0, 0, 1, NULL); //FEA

	rootTriangle[4] = new Triangle(1/NORM, -1/NORM, 1/NORM, 1/NORM, -1/NORM, -1/NORM, 1/NORM, 1/NORM, -1/NORM, 1, 0, 0, NULL); //BCG (cote gauche)
	rootTriangle[5] = new Triangle(1/NORM, 1/NORM, -1/NORM, 1/NORM, 1/NORM, 1/NORM, 1/NORM, -1/NORM, 1/NORM, 1, 0, 0, NULL); //GFB

	rootTriangle[6] = new Triangle(1/NORM, -1/NORM, -1/NORM, -1/NORM, -1/NORM, -1/NORM, -1/NORM, 1/NORM, -1/NORM, 0, 0, -1, NULL); //CDH (face arriere)
	rootTriangle[7] = new Triangle(-1/NORM, 1/NORM, -1/NORM, 1/NORM, 1/NORM, -1/NORM, 1/NORM, -1/NORM, -1/NORM, 0, 0, -1, NULL); //HGC

	rootTriangle[8] = new Triangle(-1/NORM, -1/NORM, -1/NORM, -1/NORM, -1/NORM, 1/NORM, -1/NORM, 1/NORM, 1/NORM, -1, 0, 0, NULL); //DAE (cote droit)
	rootTriangle[9] = new Triangle(-1/NORM, 1/NORM, 1/NORM, -1/NORM, 1/NORM, -1/NORM, -1/NORM, -1/NORM, -1/NORM, -1, 0, 0, NULL); //EHD

	rootTriangle[10] = new Triangle(-1/NORM, 1/NORM, 1/NORM, 1/NORM, 1/NORM, 1/NORM, 1/NORM, 1/NORM, -1/NORM, 0, 1, 0, NULL); //EFG (haut)
	rootTriangle[11] = new Triangle(1/NORM, 1/NORM, -1/NORM, -1/NORM, 1/NORM, -1/NORM, -1/NORM, 1/NORM, 1/NORM, 0, 1, 0, NULL); //GHE

	//Initialisation des voisin

	rootTriangle[0]->setVoisin(0, rootTriangle[6]);
	rootTriangle[0]->setVoisin(1, rootTriangle[1]);
	rootTriangle[0]->setVoisin(2, rootTriangle[4]);

	rootTriangle[1]->setVoisin(0, rootTriangle[2]);
	rootTriangle[1]->setVoisin(1, rootTriangle[0]);
	rootTriangle[1]->setVoisin(2, rootTriangle[8]);

	rootTriangle[2]->setVoisin(0, rootTriangle[1]);
	rootTriangle[2]->setVoisin(1, rootTriangle[3]);
	rootTriangle[2]->setVoisin(2, rootTriangle[5]);

	rootTriangle[3]->setVoisin(0, rootTriangle[10]);
	rootTriangle[3]->setVoisin(1, rootTriangle[2]);
	rootTriangle[3]->setVoisin(2, rootTriangle[8]);

	rootTriangle[4]->setVoisin(0, rootTriangle[0]);
	rootTriangle[4]->setVoisin(1, rootTriangle[5]);
	rootTriangle[4]->setVoisin(2, rootTriangle[7]);

	rootTriangle[5]->setVoisin(0, rootTriangle[10]);
	rootTriangle[5]->setVoisin(1, rootTriangle[4]);
	rootTriangle[5]->setVoisin(2, rootTriangle[2]);

	rootTriangle[6]->setVoisin(0, rootTriangle[0]);
	rootTriangle[6]->setVoisin(1, rootTriangle[7]);
	rootTriangle[6]->setVoisin(2, rootTriangle[9]);

	rootTriangle[7]->setVoisin(0, rootTriangle[11]);
	rootTriangle[7]->setVoisin(1, rootTriangle[6]);
	rootTriangle[7]->setVoisin(2, rootTriangle[4]);

	rootTriangle[8]->setVoisin(0, rootTriangle[1]);
	rootTriangle[8]->setVoisin(1, rootTriangle[9]);
	rootTriangle[8]->setVoisin(2, rootTriangle[3]);

	rootTriangle[9]->setVoisin(0, rootTriangle[11]);
	rootTriangle[9]->setVoisin(1, rootTriangle[8]);
	rootTriangle[9]->setVoisin(2, rootTriangle[6]);

	rootTriangle[10]->setVoisin(0, rootTriangle[3]);
	rootTriangle[10]->setVoisin(1, rootTriangle[11]);
	rootTriangle[10]->setVoisin(2, rootTriangle[5]);

	rootTriangle[11]->setVoisin(0, rootTriangle[7]);
	rootTriangle[11]->setVoisin(1, rootTriangle[10]);
	rootTriangle[11]->setVoisin(2, rootTriangle[9]);

	Triangle *t = rootTriangle[7];
	
	for(int i = 0 ; i < 0 ; i++)
	{
		t->split(m_radius);
		t = t->getEnfant(0);
	}
}

Sphere::~Sphere(void)
{
	for(int i = 0 ; i < 12 ; i++)
		delete rootTriangle[i];
}

void Sphere::render(Ogre::ManualObject *obj, int &nbTri, int &nbRecurse)
{

	for (int i=0 ; i<12 ; ++i) {
		int recurse = 1;
		nbRecurse = std::max(rootTriangle[i]->render(obj, nbTri, recurse), recurse);
	}
}

void Sphere::updateMesh(Ogre::Vector3 dPos, Ogre::Camera *m_cam)
{
	bool meshUpdated = false;
	for(int i = 0 ; i < 12 ; i++)
		rootTriangle[i]->splitIfNeeded(dPos, m_radius, meshUpdated, m_cam);

	for(int i = 0 ; i < 12 ; i++)
		rootTriangle[i]->mergeIfNeeded(dPos, m_radius, meshUpdated, m_cam);

	if(meshUpdated)
	{
		int nbTri = 0;
		int recurseLevel = 1;
		m_obj->clear();
		m_obj->begin("BasicWhite", Ogre::RenderOperation::OT_TRIANGLE_LIST);
		render(m_obj, nbTri, recurseLevel);
		m_obj->end();

		std::cout << "nbTri : " << nbTri << " recursion : " << recurseLevel << " distance : " << dPos.length() << std::endl;
	}
}
