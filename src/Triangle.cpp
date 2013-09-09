#include "Triangle.h"
#include "simplexnoise.h"
#include "Sphere.h"
#include <OGRE/OgreManualObject.h>
#include <OGRE/OgreCamera.h>

Vertex::Vertex(float x, float y, float z) : m_x(x), m_y(y), m_z(z)
{
    m_index = VERTEX_NOT_DEFINED;
}

Vertex::Vertex(Ogre::Vector3 vec) : m_x(vec.x), m_y(vec.y), m_z(vec.z)
{
    m_index = VERTEX_NOT_DEFINED;
}

int Vertex::getIndex(int &lastIndex, Ogre::ManualObject *obj)
{
    if(m_index == VERTEX_NOT_DEFINED)
    {
        obj->position(m_x, m_y, m_z);
        obj->colour(Ogre::ColourValue::White);
        m_index = lastIndex;
        //std::cout << "Vertice index : " << lastIndex << std::endl;
        lastIndex++;
    }

    return m_index;
}

Triangle::Triangle(Vertex *v1, Vertex *v2, Vertex *v3, Triangle *p, int recurseLevel, std::vector<Diamond*> *diamondListP, Sphere *plnt)
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
    m_planet = plnt;
	calculateNormal();
}

Triangle::Triangle(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3, float normal1, float normal2, float normal3, Triangle *p, int recurseLevel, std::vector<Diamond*> *diamondListP, Sphere *plnt)
{
    Vertex *v0 = new Vertex(x1, y1, z1);
    v[0] = v0;

    Vertex *v1 = new Vertex(x2, y2, z2);
    v[1] = v1;

    Vertex *v2 = new Vertex(x3, y3, z3);
    v[2] = v2;

	parent = p;
	enfant[0] = NULL;
	enfant[1] = NULL;
	diamond = NULL;
	diamondList = diamondListP;
    m_planet = plnt;
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
    Ogre::Vector3 v1(v[0]->m_x - v[1]->m_x, v[0]->m_y - v[1]->m_y, v[0]->m_z - v[1]->m_z);
    Ogre::Vector3 v2(v[0]->m_x - v[2]->m_x, v[0]->m_y - v[2]->m_y, v[0]->m_z - v[2]->m_z);
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

int Triangle::render(Ogre::ManualObject *obj, int &nbTri, int nbRecurse, int &lastIndex)
{
    if(enfant[0] || enfant[1])
	{		
        int r1 = enfant[0]->render(obj, nbTri, nbRecurse+1, lastIndex);
        int r2 = enfant[1]->render(obj, nbTri, nbRecurse+1, lastIndex);

		if(r1 > r2)
			return r1;
		else
			return r2;
	}
	else
	{
        obj->triangle(v[0]->getIndex(lastIndex, obj), v[1]->getIndex(lastIndex, obj), v[2]->getIndex(lastIndex, obj));

		nbTri++;
		return nbRecurse;
    }
}

void Triangle::split(float radius, PlanetNoise *pnoise)
{
	if(enfant[0] || enfant[1])
		return;

	if(voisin[1]->getVoisin(1) != this)
		voisin[1]->split(radius, pnoise);    

    float mx = (v[2]->m_x + v[0]->m_x) / 2;
    float my = (v[2]->m_y + v[0]->m_y) / 2;
    float mz = (v[2]->m_z + v[0]->m_z) / 2;

    Ogre::Vector3 vMilieu(mx, my, mz);
	vMilieu.normalise();
    float norme = radius + pnoise->noise(mx, my, mz);
	vMilieu *= norme;

	float altitude = norme - radius;
	/*if(altitude < 0)
	{
		milieu.r = 0.0f;
		milieu.g = 0.0f;
		milieu.b = 0.5f;
	}
	else if(altitude < 500)
	{
		milieu.r = 0.0f;
		milieu.g = 0.5f;
		milieu.b = 0.0f;
	}
	else if(altitude < 4000)
	{
		milieu.r = 0.36f;
		milieu.g = 0.18f;
		milieu.b = 0.12f;
	}
    else
	{
		milieu.r = 1.0f;
		milieu.g = 1.0f;
		milieu.b = 1.0f;
    }*/

    Vertex *milieu = new Vertex(vMilieu.x, vMilieu.y, vMilieu.z);

    Triangle *enfant1 = new Triangle(v[1], milieu, v[0], this, m_recurseLevel+1,diamondList, m_planet);
    Triangle *enfant2 = new Triangle(v[2], milieu, v[1], this, m_recurseLevel+1 ,diamondList, m_planet);

	enfant[0] = enfant1;
	enfant[1] = enfant2;

	Triangle *tVoisin = voisin[1];

    Triangle *enfantV1 = new Triangle(tVoisin->v[1], milieu, tVoisin->v[0], tVoisin, m_recurseLevel+1, diamondList, m_planet);
    Triangle *enfantV2 = new Triangle(tVoisin->v[2], milieu, tVoisin->v[1], tVoisin, m_recurseLevel+1, diamondList, m_planet);

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

    delete enfant1->v[1];

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

bool Triangle::needsSplit(Ogre::Vector3 dPos, Ogre::Camera *m_cam)
{
    if(m_recurseLevel < 9)
        return true;
		Ogre::Vector3 camPos = m_cam->getPosition();
		camPos.normalise();
		
        float cosHorizon = m_planet->getRadius() / m_cam->getPosition().length();

		float val  = camPos.dotProduct(normal) - cosHorizon;

		if(val < 0) // le triangle est cache car il est sur l'autre face de la planete
			return false;

		Ogre::Vector3 milieu;
        milieu.x = (v[2]->m_x + v[0]->m_x) / 2;
        milieu.y = (v[2]->m_y + v[0]->m_y) / 2;
        milieu.z = (v[2]->m_z + v[0]->m_z) / 2;
		Ogre::Vector3 distance = milieu - m_cam->getPosition();

        //float distance_value = distance.length();

		//if(!m_cam->isVisible(Ogre::Vector3(v[0].x, v[0].y, v[0].z)) && !m_cam->isVisible(Ogre::Vector3(v[1].x, v[1].y, v[1].z)) && !m_cam->isVisible(Ogre::Vector3(v[2].x, v[2].y, v[2].z)) && distance_value > 150.0f)
		//	return false; //Le triangle est hors champ

		Ogre::Vector3 edge;
        edge.x = v[2]->m_x - v[0]->m_x;
        edge.y = v[2]->m_y - v[0]->m_y;
        edge.z = v[2]->m_z - v[0]->m_z;

		if (edge.squaredLength() < 1)  return false;

        edge *= (/*std::powf(val, 2)+*/4/std::pow(val+1,2));
		return (edge.squaredLength()/distance.squaredLength()*15) > 1;

		/*Ogre::Vector2 v2 = getScreenCoordinate(Ogre::Vector3(v[2].x, v[2].y, v[2].z), m_cam), v0 = getScreenCoordinate(Ogre::Vector3(v[0].x, v[0].y, v[0].z), m_cam);
		
		float sqL = (v2 - v0).length();
		//std::cout << sqL << std::endl;
		return sqL > 1000;*/
}

Ogre::Vector2 Triangle::getScreenCoordinate(Ogre::Vector3 vertex, Ogre::Camera *m_cam)
{
	Ogre::Vector4 transformedVector = Ogre::Vector4(vertex) * m_cam->getViewMatrix(true);
	Ogre::Vector4 screenProjectedVector = transformedVector * m_cam->getProjectionMatrix();
	return Ogre::Vector2(screenProjectedVector.x, screenProjectedVector.y);
}

void Triangle::splitIfNeeded(Ogre::Vector3 dPos, float radius, bool &meshUpdated, Ogre::Camera *m_cam, PlanetNoise *pnoise)
{
    v[0]->reinitIndex();
    v[1]->reinitIndex();
    v[2]->reinitIndex();

	if(enfant[0] || enfant[1])
	{
		if(enfant[0])
			enfant[0]->splitIfNeeded(dPos, radius, meshUpdated, m_cam, pnoise);
		if(enfant[1])
			enfant[1]->splitIfNeeded(dPos, radius, meshUpdated, m_cam, pnoise);
	}
	else
	{
            meshUpdated = false;
        if (needsSplit(dPos, m_cam)) {
			split(radius, pnoise);
			splitIfNeeded(dPos, radius, meshUpdated, m_cam, pnoise);
            meshUpdated = true;
		}
	}
}

float Triangle::error(Ogre::Vector3 dPos, Ogre::Camera *m_cam, float radius)
{
	float distance = dPos.squaredDistance(Ogre::Vector3::ZERO);

	Ogre::Vector3 milieu;
    milieu.x = (v[2]->m_x + v[0]->m_x) / 2;
    milieu.y = (v[2]->m_y + v[0]->m_y) / 2;
    milieu.z = (v[2]->m_z + v[0]->m_z) / 2;

	float fov = m_cam->getDirection().dotProduct((milieu - dPos).normalisedCopy());

    float edge_lenght = v[2]->m_x * v[0]->m_x + v[2]->m_y * v[0]->m_y + v[2]->m_z * v[0]->m_z;
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
    if(!m_cam->isVisible(Ogre::Vector3(v[0]->m_x, v[0]->m_y, v[0]->m_z)) && !m_cam->isVisible(Ogre::Vector3(v[1]->m_x, v[1]->m_y, v[1]->m_z)) && !m_cam->isVisible(Ogre::Vector3(v[2]->m_x, v[2]->m_y, v[2]->m_z)))
		return false; //Le triangle est hors champ
	else
		return true;
}
