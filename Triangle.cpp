#include "Triangle.h"
#include "simplexnoise.h"
#include <OGRE\OgreManualObject.h>
#include <OGRE\OgreCamera.h>

Triangle::Triangle(Vertex v1, Vertex v2, Vertex v3, Triangle *p, int recurseLevel, std::vector<Diamond*> *diamondListP)
{
	v[0] = v1;
	v[1] = v2;
	v[2] = v3;

	parent = p;
	enfant[0] = NULL;
	enfant[1] = NULL;
	diamond = NULL;
	diamondList = diamondListP;
	m_recurseLevel = recurseLevel;
	calculateNormal();
}

Triangle::Triangle(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3, float normal1, float normal2, float normal3, Triangle *p, int recurseLevel, std::vector<Diamond*> *diamondListP)
{
	v[0].x = x1;
	v[0].y = y1;
	v[0].z = z1;

	v[0].nx = normal1;
	v[0].ny = normal2;
	v[0].nz = normal3;

	v[1].x = x2;
	v[1].y = y2;
	v[1].z = z2;

	v[1].nx = normal1;
	v[1].ny = normal2;
	v[1].nz = normal3;

	v[2].x = x3;
	v[2].y = y3;
	v[2].z = z3;

	v[2].nx = normal1;
	v[2].ny = normal2;
	v[2].nz = normal3;

	parent = p;
	enfant[0] = NULL;
	enfant[1] = NULL;
	diamond = NULL;
	diamondList = diamondListP;
	m_recurseLevel = recurseLevel;
	calculateNormal();
}

Triangle::~Triangle(void)
{
	if(enfant[0])
		delete enfant[0];
	if(enfant[1])
		delete enfant[1];
}

void Triangle::calculateNormal()
{
	Ogre::Vector3 v1(v[0].x - v[1].x, v[0].y - v[1].y, v[0].z - v[1].z);
	Ogre::Vector3 v2(v[0].x - v[2].x, v[0].y - v[2].y, v[0].z - v[2].z);
	normal = v1.crossProduct(v2);
	normal.normalise();
}

void Triangle::setVoisin(int vNum, Triangle *v)
{
	voisin[vNum] = v;
}

Triangle* Triangle::getVoisin(int vNum)
{
	return voisin[vNum];
}

void Triangle::setEnfant(int enfantNum, Triangle *e)
{
	enfant[enfantNum] = e;
}

Triangle *Triangle::getEnfant(int enfantNum)
{
	return enfant[enfantNum];
}

Triangle* Triangle::getParent()
{
	return parent;
}

int Triangle::render(Ogre::ManualObject *obj, int &nbTri, int nbRecurse)
{
	if(enfant[0] || enfant[1])
	{		
		int r1 = enfant[0]->render(obj, nbTri, nbRecurse+1);
		int r2 = enfant[1]->render(obj, nbTri, nbRecurse+1);

		if(r1 > r2)
			return r1;
		else
			return r2;
	}
	else
	{
		obj->position(v[0].x, v[0].y, v[0].z);
		obj->normal(normal);
		obj->position(v[1].x, v[1].y, v[1].z);
		obj->normal(normal);
		obj->position(v[2].x, v[2].y, v[2].z);
		obj->normal(normal);
		nbTri++;
		return nbRecurse;
	}
}

void Triangle::split(float radius)
{
	if(enfant[0] || enfant[1])
		return;

	if(voisin[1]->getVoisin(1) != this)
		voisin[1]->split(radius);

	Vertex milieu;

	milieu.x = (v[2].x + v[0].x) / 2;
	milieu.y = (v[2].y + v[0].y) / 2;
	milieu.z = (v[2].z + v[0].z) / 2;

	Ogre::Vector3 vMilieu(milieu.x, milieu.y, milieu.z);
	vMilieu.normalise();
	vMilieu *= radius + scaled_raw_noise_3d(0, 12, milieu.x/80, milieu.y/80, milieu.z/80);

	milieu.x = vMilieu.x;
	milieu.y = vMilieu.y;
	milieu.z = vMilieu.z;

	milieu.nx = v[0].nx;
	milieu.ny = v[0].ny;
	milieu.nz = v[0].nz;

	Triangle *enfant1 = new Triangle(v[1], milieu, v[0], this, m_recurseLevel+1,diamondList);
	Triangle *enfant2 = new Triangle(v[2], milieu, v[1], this, m_recurseLevel+1 ,diamondList);

	enfant[0] = enfant1;
	enfant[1] = enfant2;

	Triangle *tVoisin = voisin[1];

	Triangle *enfantV1 = new Triangle(tVoisin->v[1], milieu, tVoisin->v[0], tVoisin, m_recurseLevel+1, diamondList);
	Triangle *enfantV2 = new Triangle(tVoisin->v[2], milieu, tVoisin->v[1], tVoisin, m_recurseLevel+1, diamondList);

	tVoisin->enfant[0] = enfantV1;
	tVoisin->enfant[1] = enfantV2;

	enfant1->voisin[0] = enfant2;
	enfant1->voisin[1] = voisin[0];
	enfant1->voisin[2] = enfantV2;
	//voisin[0]->voisin[0] = enfant1; - Ca marche que dans un cas sur deux
	for(int i = 0 ; i < 3 ; i++)
	{
		if(voisin[0]->voisin[i] == this)
			voisin[0]->voisin[i] = enfant1;
	}

	enfant2->voisin[0] = enfantV1;
	enfant2->voisin[1] = voisin[2];
	for(int i = 0 ; i < 3 ; i++)
	{
		if(voisin[2]->voisin[i] == this)
			voisin[2]->voisin[i] = enfant2;
	}
	enfant2->voisin[2] = enfant1;

	enfantV1->voisin[0] = enfantV2;
	enfantV1->voisin[1] = voisin[1]->voisin[0];
	enfantV1->voisin[2] = enfant2;
	//voisin[1]->voisin[0]->voisin[0] = enfantV1; - Pareil qu'en haut
	for(int i = 0 ; i < 3 ; i++)
	{
		if(voisin[1]->voisin[0]->voisin[i] == voisin[1])
			voisin[1]->voisin[0]->voisin[i] = enfantV1;
	}

	enfantV2->voisin[0] = enfant1;
	enfantV2->voisin[1] = voisin[1]->voisin[2];
	for(int i = 0 ; i < 3 ; i++)
	{
		if(voisin[1]->voisin[2]->voisin[i] == voisin[1])
			voisin[1]->voisin[2]->voisin[i] = enfantV2;
	}
	enfantV2->voisin[2] = enfantV1;

	if(diamond)
	{
		//diamondList->remove(diamond);
		diamond->removed = true;
		diamond->nullAllTriangle();		
		//delete diamond;
	}

	if(voisin[1]->diamond)
	{
		/*diamondList->remove(voisin[1]->diamond);
		delete voisin[1]->diamond;*/
		voisin[1]->diamond->removed = true;
		voisin[1]->diamond->nullAllTriangle();		
	}

	enfant1->diamond = new Diamond(enfant1);
	diamondList->push_back(enfant1->diamond);
}

//#define MERGE_VOISIN(x) for(int i = 0 ; i < 3 ; i++) { if(!(x->voisin[i] == enfant2 || x->voisin[i] == enfantV1 || x->voisin[i] == enfantV2 || x->voisin[i] == enfant2)) { for(int j = 0 ; j < 3 ; j++) { if(x->voisin[i]->voisin[j] == x) x->voisin[i]->voisin[j] = x->parent; } } }

void Triangle::merge()
{
	//std::cout << "Call merge" << std::endl;
	/*if(!parent) //Root triangle can't be merged
		return;

	if(enfant[1] || enfant[0])
		return;*/

	if(voisin[0]->voisin[0]->voisin[0]->voisin[0] != this)
		return;

	//std::cout << "Merging triangles" << std::endl;

	Triangle *parentVoisin = parent->voisin[1];

	Triangle *enfant1 = parent->enfant[0];
	Triangle *enfant2 = parent->enfant[1];
	Triangle *enfantV1 = parentVoisin->enfant[0];
	Triangle *enfantV2 = parentVoisin->enfant[1];

	for(int i = 0 ; i < 3 ; i++)
	{
		if(enfant1->voisin[1]->voisin[i] == enfant1)
		{
			enfant1->voisin[1]->voisin[i] = parent;
			parent->voisin[0] = enfant1->voisin[1];
		}
	}

	for(int i = 0 ; i < 3 ; i++)
	{
		if(enfant2->voisin[1]->voisin[i] == enfant2)
		{
			enfant2->voisin[1]->voisin[i] = parent;
			parent->voisin[2] = enfant2->voisin[1];
		}
	}

	for(int i = 0 ; i < 3 ; i++)
	{
		if(enfantV1->voisin[1]->voisin[i] == enfantV1)
		{
			enfantV1->voisin[1]->voisin[i] = parentVoisin;
			parentVoisin->voisin[0] = enfantV1->voisin[1];
		}
	}

	for(int i = 0 ; i < 3 ; i++)
	{
		if(enfantV2->voisin[1]->voisin[i] == enfantV2)
		{
			enfantV2->voisin[1]->voisin[i] = parentVoisin;
			parentVoisin->voisin[2] = enfantV2->voisin[1];
		}
	}
	
	parent->enfant[0] = NULL;
	parent->enfant[1] = NULL;
	parentVoisin->enfant[0] = NULL;
	parentVoisin->enfant[1] = NULL;

	enfant1->diamond->removed = true;
	enfant1->diamond->nullAllTriangle();	

	if(Triangle::belongToADiamond(parent))
	{
		parent->diamond = new Diamond(parent);
		diamondList->push_back(parent->diamond);
	}

	if(Triangle::belongToADiamond(parentVoisin))
	{
		parentVoisin->diamond = new Diamond(parentVoisin);
		diamondList->push_back(parentVoisin->diamond);
	}

	delete enfant1;
	delete enfant2;
	delete enfantV1;
	delete enfantV2;
}

float Triangle::ratio(Ogre::Camera *m_cam)
{

	Ogre::Vector3 milieu;
	milieu.x = (v[2].x + v[0].x) / 2;
	milieu.y = (v[2].y + v[0].y) / 2;
	milieu.z = (v[2].z + v[0].z) / 2;
	Ogre::Vector3 distance = milieu - m_cam->getPosition();

	Ogre::Vector3 edge;
	edge.x = v[2].x - v[0].x;
	edge.y = v[2].y - v[0].y;
	edge.z = v[2].z - v[0].z;

	

	//std::cout << edge.length()/distance.length()*100 << std::endl;
	//return 0;
	return edge.length()/distance.length()*30;
	//return 1000*((edge.squaredLength()) / distance.squaredLength());
}

bool Triangle::needsSplit(Ogre::Vector3 dPos, bool &meshUpdated, Ogre::Camera *m_cam) {		

		float val  = m_cam->getDirection().dotProduct(-normal);

		if(m_recurseLevel < 9)
			return true;

		if(val <= 0) // le triangle est cache car il est sur l'autre face de la planete
			return false;		

		Ogre::Vector3 milieu;
		milieu.x = (v[2].x + v[0].x) / 2;
		milieu.y = (v[2].y + v[0].y) / 2;
		milieu.z = (v[2].z + v[0].z) / 2;
		Ogre::Vector3 distance = milieu - m_cam->getPosition();

		float distance_value = distance.length();

		if(!m_cam->isVisible(Ogre::Vector3(v[0].x, v[0].y, v[0].z)) && !m_cam->isVisible(Ogre::Vector3(v[1].x, v[1].y, v[1].z)) && !m_cam->isVisible(Ogre::Vector3(v[2].x, v[2].y, v[2].z)) && distance_value > 150.0f)
			return false; //Le triangle est hors champ

		Ogre::Vector3 edge;
		edge.x = v[2].x - v[0].x;
		edge.y = v[2].y - v[0].y;
		edge.z = v[2].z - v[0].z;

		if (edge.squaredLength() < 1)  return false;

		//std::cout << edge.length()/distance.length()*100 << std::endl;

		
		//if (val > 0.9)
		//	std::cout << val << std::endl;

		edge *= (std::powf(val, 8)+3/std::powf(val+1,8));
		//return 0;
		return (edge.length()/distance_value*15) > 1;
}

void Triangle::splitIfNeeded(Ogre::Vector3 dPos, float radius, bool &meshUpdated, Ogre::Camera *m_cam)
{
	if(enfant[0] || enfant[1])
	{
		if(enfant[0])
			enfant[0]->splitIfNeeded(dPos, radius, meshUpdated, m_cam);
		if(enfant[1])
			enfant[1]->splitIfNeeded(dPos, radius, meshUpdated, m_cam);
	}
	else
	{
			meshUpdated = true;
		if (needsSplit(dPos, meshUpdated, m_cam)) {
			split(radius);
		}
	}
}

void Triangle::mergeIfNeeded(Ogre::Vector3 dPos, float radius, bool &meshUpdated, Ogre::Camera *m_cam)
{
	if(enfant[0] || enfant[1])
	{
		if(enfant[0])
			enfant[0]->mergeIfNeeded(dPos, radius, meshUpdated, m_cam);
		if(enfant[1] && !meshUpdated)
			enfant[1]->mergeIfNeeded(dPos, radius, meshUpdated, m_cam);
	}
	else
	{
		//float ratio = variance(dPos, radius);
		
		//std::cout << "Ratio: " << ratio << std::endl;

		/*if(!m_cam->isVisible(Ogre::Vector3(v[0].x, v[0].y, v[0].z)) && !m_cam->isVisible(Ogre::Vector3(v[1].x, v[1].y, v[1].z)) && !m_cam->isVisible(Ogre::Vector3(v[2].x, v[2].y, v[2].z)))
		{
			merge();
			meshUpdated = true;
			return;
		}

		if(ratio < 0.0001f && (abs(ratio - 0.0001f) / 0.0001) > 0.10f)
		{
			merge();
			meshUpdated = true;
		}*/		
	}
}

float Triangle::error(Ogre::Vector3 dPos, Ogre::Camera *m_cam, float radius)
{
	float distance = dPos.squaredDistance(Ogre::Vector3::ZERO);

	Ogre::Vector3 milieu;
	milieu.x = (v[2].x + v[0].x) / 2;
	milieu.y = (v[2].y + v[0].y) / 2;
	milieu.z = (v[2].z + v[0].z) / 2;

	float fov = m_cam->getDirection().dotProduct((milieu - dPos).normalisedCopy());

	float edge_lenght = v[2].x * v[0].x + v[2].y * v[0].y + v[2].z * v[0].z;
	float altitude = dPos.length() - radius;
	if(altitude < 1.0f)
		altitude = 1.0f;

	float horizon = altitude * altitude + 2.0f * altitude * radius;

	float error = 0.0f;

	//if(distance < edge_lenght || (distance < horizon && fov > (1 - m_cam->getFOVy().valueRadians()))

	return 0.0f;

}

bool Triangle::belongToADiamond(Triangle *t)
{
	return t->voisin[0]->voisin[0]->voisin[0]->voisin[0] == t;
}

bool Triangle::isVisible(Ogre::Camera *m_cam)
{
	if(!m_cam->isVisible(Ogre::Vector3(v[0].x, v[0].y, v[0].z)) && !m_cam->isVisible(Ogre::Vector3(v[1].x, v[1].y, v[1].z)) && !m_cam->isVisible(Ogre::Vector3(v[2].x, v[2].y, v[2].z)))
		return false; //Le triangle est hors champ
	else
		return true;
}
