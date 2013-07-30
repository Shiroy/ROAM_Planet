#include "Sphere.h"
#include "Triangle.h"
#include <OGRE\OgreCamera.h>
#include <math.h>

#define NORM Ogre::Math::Sqrt(3) * m_radius

Sphere::Sphere(float radius, Ogre::ManualObject *obj, Ogre::Camera *cam, Ogre::SceneNode *node, PlanetNoise *pnoise) : m_radius(radius), m_pnoise(pnoise)
{
	m_diamondList.clear();
	m_obj = obj;
	m_node = node;
	m_meshUpdated = false;
	rootTriangle[0] = new Triangle(-1/NORM, -1/NORM, -1/NORM, 1/NORM, -1/NORM, -1/NORM, 1/NORM, -1/NORM, 1/NORM, 0, -1, 0, NULL, 1, &m_diamondList); //DCB (dessous)
	rootTriangle[1] = new Triangle(1/NORM, -1/NORM, 1/NORM, -1/NORM, -1/NORM, 1/NORM, -1/NORM, -1/NORM, -1/NORM, 0, -1, 0, NULL, 1, &m_diamondList); //BAD

	rootTriangle[2] = new Triangle(-1/NORM, -1/NORM, 1/NORM, 1/NORM, -1/NORM, 1/NORM, 1/NORM, 1/NORM, 1/NORM, 0, 0, 1, NULL, 1, &m_diamondList); //ABF (face avant)
	rootTriangle[3] = new Triangle(1/NORM, 1/NORM, 1/NORM, -1/NORM, 1/NORM, 1/NORM, -1/NORM, -1/NORM, 1/NORM, 0, 0, 1, NULL, 1, &m_diamondList); //FEA

	rootTriangle[4] = new Triangle(1/NORM, -1/NORM, 1/NORM, 1/NORM, -1/NORM, -1/NORM, 1/NORM, 1/NORM, -1/NORM, 1, 0, 0, NULL, 1, &m_diamondList); //BCG (cote gauche)
	rootTriangle[5] = new Triangle(1/NORM, 1/NORM, -1/NORM, 1/NORM, 1/NORM, 1/NORM, 1/NORM, -1/NORM, 1/NORM, 1, 0, 0, NULL, 1, &m_diamondList); //GFB

	rootTriangle[6] = new Triangle(1/NORM, -1/NORM, -1/NORM, -1/NORM, -1/NORM, -1/NORM, -1/NORM, 1/NORM, -1/NORM, 0, 0, -1, NULL, 1, &m_diamondList); //CDH (face arriere)
	rootTriangle[7] = new Triangle(-1/NORM, 1/NORM, -1/NORM, 1/NORM, 1/NORM, -1/NORM, 1/NORM, -1/NORM, -1/NORM, 0, 0, -1, NULL, 1, &m_diamondList); //HGC

	rootTriangle[8] = new Triangle(-1/NORM, -1/NORM, -1/NORM, -1/NORM, -1/NORM, 1/NORM, -1/NORM, 1/NORM, 1/NORM, -1, 0, 0, NULL, 1, &m_diamondList); //DAE (cote droit)
	rootTriangle[9] = new Triangle(-1/NORM, 1/NORM, 1/NORM, -1/NORM, 1/NORM, -1/NORM, -1/NORM, -1/NORM, -1/NORM, -1, 0, 0, NULL, 1, &m_diamondList); //EHD

	rootTriangle[10] = new Triangle(-1/NORM, 1/NORM, 1/NORM, 1/NORM, 1/NORM, 1/NORM, 1/NORM, 1/NORM, -1/NORM, 0, 1, 0, NULL, 1, &m_diamondList); //EFG (haut)
	rootTriangle[11] = new Triangle(1/NORM, 1/NORM, -1/NORM, -1/NORM, 1/NORM, -1/NORM, -1/NORM, 1/NORM, 1/NORM, 0, 1, 0, NULL, 1, &m_diamondList); //GHE

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
		t->split(m_radius, pnoise);
		t = t->getEnfant(0);
	}

	firstDraw = true;

	pthread_mutex_init(&m_mutex, NULL);

	updateThreadArg *arg = new updateThreadArg;
	arg->thisPointer = this;
	arg->m_cam = cam;
	pthread_create(&m_thread, NULL, updateThread, (void*)arg);
}

Sphere::~Sphere(void)
{
	for(int i = 0 ; i < 12 ; i++)
		delete rootTriangle[i];
}

void Sphere::renderIfUpdated()
{
	if(m_meshUpdated)
	{
		pthread_mutex_lock(&m_mutex);
		int nbTri = 0;
		int recurseLevel = 1;
		/*if(firstDraw)
		{
			m_obj->begin("cube", Ogre::RenderOperation::OT_TRIANGLE_LIST);
			firstDraw = false;
		}
		else
			m_obj->beginUpdate(0);*/
		render(m_obj, nbTri, recurseLevel);
		//m_obj->end();

		//std::cout << "nbTri : " << nbTri << std::endl;
		m_meshUpdated = false;
		pthread_mutex_unlock(&m_mutex);
	}
}

void Sphere::render(Ogre::ManualObject *obj, int &nbTri, int &nbRecurse)
{
	for (int i=0 ; i<12 ; ++i) {
		int recurse = 1;
		if(firstDraw)
			obj->begin("cube", Ogre::RenderOperation::OT_TRIANGLE_LIST);
		else
			obj->beginUpdate(i);
		nbRecurse = std::max(rootTriangle[i]->render(obj, nbTri, recurse), recurse);
		obj->end();
	}

	if(firstDraw)
		firstDraw = false;
}

bool Sphere::updateMesh(Ogre::Vector3 dPos, Ogre::Camera *m_cam)
{
	bool meshUpdated = false;
	for(int i = 0 ; i < 12 ; i++)
		rootTriangle[i]->splitIfNeeded(dPos, m_radius, meshUpdated, m_cam, m_pnoise);

	//std::cout << "Diamond : " << m_diamondList.size() << " ";

	int tick = GetTickCount();
	int newTick = 0;

	for(int i = 0 ; i != m_diamondList.size() ; ++i)
	{
		Diamond *dia = m_diamondList[i];
		if(dia->canBeMerged(m_cam))
		{
			//std::cout << "Merge" << std::endl;
			dia->pTriComposed[0]->merge();
			meshUpdated = true;
		}
		else if(dia->removed)
		{			
			m_diamondList.erase(m_diamondList.begin()+i);
			i--;
		}
	}
	newTick = GetTickCount();
	//std::cout << "Extracting object done (diff : " << newTick - tick << ")" << std::endl;
	tick = newTick;

	return meshUpdated;
}

void *Sphere::updateThread(void *arg)
{
	updateThreadArg *tArg = (updateThreadArg*)arg;
	Sphere *caller = tArg->thisPointer;
	Ogre::Camera *m_cam = tArg->m_cam;
	delete tArg;

	int tickCount = GetTickCount(); //update timer
	int lastTick = 0;

	while(true)
	{
		tickCount = GetTickCount();
		if(tickCount - lastTick < 250)
			continue;

		Ogre::Vector3 dPos = caller->m_node->getPosition() - m_cam->getPosition();

		pthread_mutex_lock(&caller->m_mutex);
		caller->m_meshUpdated = caller->updateMesh(dPos, m_cam);
		pthread_mutex_unlock(&caller->m_mutex);
		lastTick = GetTickCount();
	}

}
