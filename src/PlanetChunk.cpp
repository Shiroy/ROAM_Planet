#include "PlanetChunk.h"
#include "Demarage.h"


PlanetChunk::PlanetChunk(Ogre::Vector3 bottomLeftCorner, Ogre::Vector3 bottomRightCorner, Ogre::Vector3 topRightCorner, Ogre::Vector3 topLefCorner, float radius, Ogre::SceneNode *node, Ogre::SceneManager *scnMgr)
{
    m_blC = bottomLeftCorner;
    m_brC = bottomRightCorner;
    m_tlC = topLefCorner;
    m_trC = topRightCorner;

    m_node = node;
    m_sceneManager = scnMgr;
    m_radius = radius;

    Ogre::Vector3 vecH = ((m_brC - m_blC) / VERTEX_PER_CHUNK);
    Ogre::Vector3 vecV = ((m_tlC - m_blC) / VERTEX_PER_CHUNK);

    m_obj = m_sceneManager->createManualObject();

    m_obj->begin("BasicWhite", Ogre::RenderOperation::OT_TRIANGLE_LIST);

    for(int i = 0 ; i <= VERTEX_PER_CHUNK ; i++)
    {
        for(int j = 0 ; j <= VERTEX_PER_CHUNK ; j++)
        {
            Ogre::Vector3 vPos = m_blC + (j * vecH) + (i * vecV);
            vPos.normalise();
            vPos *= radius;
            m_obj->position(vPos);
        }
    }

    for(int i = 0 ; i < VERTEX_PER_CHUNK ; i++)
    {
        for(int j = 0 ; j < VERTEX_PER_CHUNK ; j++)
        {
            m_obj->triangle((i+1)*(VERTEX_PER_CHUNK+1) + j, i*(VERTEX_PER_CHUNK+1) + j, i*(VERTEX_PER_CHUNK+1) + j + 1);
            m_obj->triangle(i*(VERTEX_PER_CHUNK+1) + j + 1, (i+1)*(VERTEX_PER_CHUNK+1) + j + 1, (i+1)*(VERTEX_PER_CHUNK+1) + j);
        }
    }

    m_obj->end();

    m_node->attachObject(m_obj);
}
