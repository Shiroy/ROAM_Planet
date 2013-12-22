#include "PlanetChunk.h"
#include "Demarage.h"
#include "Planet.h"
#include "PlanetNoise.h"

PlanetChunk::PlanetChunk(Ogre::Vector3 bottomLeftCorner, Ogre::Vector3 bottomRightCorner, Ogre::Vector3 topRightCorner, Ogre::Vector3 topLefCorner, float radius, Ogre::SceneNode *node, Ogre::SceneManager *scnMgr, Ogre::Camera *cam, float error, Planet* planet, int currentDepth)
{
    m_blC = bottomLeftCorner;
    m_brC = bottomRightCorner;
    m_tlC = topLefCorner;
    m_trC = topRightCorner;

    m_plane = OGRE_NEW Ogre::Plane(m_tlC, m_blC, m_brC);
    m_boundedVolume.planes.push_back(*m_plane);

    m_node = node;
    m_sceneManager = scnMgr;    
    m_camera = cam;
    m_error = error;
    m_planet = planet;
    m_radius = m_planet->getRadius();
    m_maxDepth = (int) (Ogre::Math::Log2((2*m_radius)/(VERTEX_PER_CHUNK*MIN_LENGHT)) + 1);
    m_currentDepth = currentDepth;

    m_centralPoint = (m_blC + m_trC)/2;
    centralPointExact = m_centralPoint.normalisedCopy();
    float cnoise = planet->noise()->noise(centralPointExact.x, centralPointExact.y, centralPointExact.z, radius);
    centralPointExact *= (radius+cnoise);
    m_edgeLenght = (m_blC - m_trC).length();

    Ogre::Vector3 vecV = ((m_blC - m_tlC) / (VERTEX_PER_CHUNK-1));
    Ogre::Vector3 vecH = ((m_trC - m_tlC) / (VERTEX_PER_CHUNK-1));

    for(int i = 0 ; i < 4 ; i++)
        m_childs[i] = 0;

    m_obj = m_sceneManager->createManualObject();

    m_obj->begin("cube", Ogre::RenderOperation::OT_TRIANGLE_LIST);

    Ogre::Vector3 vertexList[VERTEX_PER_CHUNK][VERTEX_PER_CHUNK];

    for(int i = 0 ; i < VERTEX_PER_CHUNK ; i++)
    {
        for(int j = 0 ; j < VERTEX_PER_CHUNK ; j++)
        {
            Ogre::Vector3 vPos = m_tlC + (j * vecH) + (i * vecV);
            vPos.normalise();
            float noise = planet->noise()->noise(vPos.x, vPos.y, vPos.z, radius);
            vPos *= (radius+noise);
            vertexList[i][j] = vPos;
            //m_obj->position(vPos);
        }
    }

    //Calcul des normal

    for(int i = 0 ; i < VERTEX_PER_CHUNK ; i++)
    {
        for(int j = 0 ; j < VERTEX_PER_CHUNK ; j++)
        {
            Ogre::Vector3 p1, p2, p3;
            p1 = vertexList[i][j];
            p2 = vertexList[i][j+1];
            p3 = vertexList[i+1][j];

            Ogre::Vector3 v1 = p3 - p1;
            Ogre::Vector3 v2 = p2 - p1;
            Ogre::Vector3 normal = v1.crossProduct(v2);
            normal.normalise();
            m_obj->position(p1);
            m_obj->normal(normal);
        }
    }

    //std::cout << "Error : " << m_error << std::endl;

    for(int i = 0 ; i < VERTEX_PER_CHUNK-1 ; i++)
    {
        for(int j = 0 ; j < VERTEX_PER_CHUNK-1 ; j++)
        {
            //m_obj->triangle((i+1)*(VERTEX_PER_CHUNK) + j, i*(VERTEX_PER_CHUNK) + j, i*(VERTEX_PER_CHUNK) + j + 1);
            //m_obj->triangle(i*(VERTEX_PER_CHUNK) + j + 1, (i+1)*(VERTEX_PER_CHUNK) + j + 1, (i+1)*(VERTEX_PER_CHUNK) + j);
            //m_obj->triangle(i*VERTEX_PER_CHUNK+j, i*VERTEX_PER_CHUNK+j+1, (i+1)*VERTEX_PER_CHUNK+j);
            //m_obj->triangle(i*VERTEX_PER_CHUNK*j+1, (i+1)*VERTEX_PER_CHUNK+j+1, (i+1)*VERTEX_PER_CHUNK+j);

            m_obj->index((i+1)*VERTEX_PER_CHUNK+j);
            m_obj->index((i+1)*VERTEX_PER_CHUNK+(j+1));
            m_obj->index(i*VERTEX_PER_CHUNK+(j+1));

            m_obj->index(i*VERTEX_PER_CHUNK+(j+1));
            m_obj->index(i*VERTEX_PER_CHUNK+j);
            m_obj->index((i+1)*VERTEX_PER_CHUNK+(j));
        }
    }

    m_obj->end();

    m_node->attachObject(m_obj);
}

PlanetChunk::~PlanetChunk()
{
    m_node->detachObject(m_obj);
    m_sceneManager->destroySceneNode(m_node);
    m_sceneManager->destroyManualObject(m_obj);

    for(int i = 0 ; i < 4 ; i++)
    {
        if(m_childs[i])
        {
            delete m_childs[i];
            m_childs[i] = 0;
        }
    }

    OGRE_DELETE m_plane;
}

void PlanetChunk::update()
{
    /*if(m_camera->getPosition().dotProduct(centralPointExact) < 0)
    {
        m_node->setVisible(false);
        return;
    }
    else
        m_node->setVisible(true);*/

    if(!needMerge()) //Reorganiser cette merde
    {
        if(!m_childs[0]) //Is a leaf node
        {
            if(needSplit())
            {
                std::cout << "Create childs" << std::endl;

                Ogre::Vector3 middleTop = (m_tlC + m_trC) / 2;
                Ogre::Vector3 middleBottom = (m_blC + m_brC) / 2;
                Ogre::Vector3 middleLeft = (m_tlC + m_blC) / 2;
                Ogre::Vector3 middleRight = (m_trC + m_brC) / 2;

                m_childs[0] = new PlanetChunk(middleLeft, m_centralPoint, middleTop, m_tlC, m_radius, m_node->createChildSceneNode(), m_sceneManager, m_camera, m_error/2, m_planet, m_currentDepth+1);
                m_childs[1] = new PlanetChunk(m_centralPoint, middleRight, m_trC, middleTop, m_radius, m_node->createChildSceneNode(), m_sceneManager, m_camera, m_error/2, m_planet, m_currentDepth+1);
                m_childs[2] = new PlanetChunk(middleBottom, m_brC, middleRight, m_centralPoint, m_radius, m_node->createChildSceneNode(), m_sceneManager, m_camera, m_error/2, m_planet, m_currentDepth+1);
                m_childs[3] = new PlanetChunk(m_blC, middleBottom, m_centralPoint, middleLeft, m_radius, m_node->createChildSceneNode(), m_sceneManager, m_camera, m_error/2, m_planet, m_currentDepth+1);

                m_node->setVisible(false, false);
            }
        }
        else
        {
            for(int i = 0 ; i < 4 ; i++)
            {
                m_childs[i]->update();
            }
        }
    }
    else
    {
        //std::cout << "No need to split dst edgeLenght : " << distance << " " << m_edgeLenght << std::endl;
        for(int i = 0 ; i < 4 ; i++)
        {
            if(m_childs[i])
            {
                delete m_childs[i];
                m_childs[i] = 0;
            }
        }

        m_node->setVisible(true, false);
    }
}

bool PlanetChunk::needSplit()
{
    float distance = (centralPointExact - m_camera->getPosition()).length();

    int neededLevel = m_maxDepth;
    while(neededLevel > 0)
    {
        if(distance < Ogre::Math::Pow(2, m_maxDepth-neededLevel) * MAX_DEPTH_ALTITUDE)
            return (neededLevel > m_currentDepth);

        neededLevel--;

    }

    std::cout << "Needed level / current level" << m_currentDepth << std::endl;

    return false;
}

bool PlanetChunk::needMerge()
{
    float distance = (centralPointExact - m_camera->getPosition()).length();

    //std::cout << "Max depth " << m_maxDepth << std::endl;

    if(distance > (Ogre::Math::Pow(2, m_maxDepth-m_currentDepth-1) * MAX_DEPTH_ALTITUDE))
    {
        //std::cout << "Need merge" << std::endl;
        return true;
    }
    return false;
}
