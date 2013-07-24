#pragma once

class Triangle;

namespace Ogre
{
	class Camera;
};

class Diamond
{
public:
	Diamond(Triangle *t1, Triangle *t2, Triangle *t3, Triangle *t4);
	Diamond(Triangle *t);
	~Diamond(void);

	bool canBeMerged(Ogre::Camera *m_cam);
	bool isDiamondVisible(Ogre::Camera *m_cam);
	void nullAllTriangle();

	Triangle *pTriComposed[4];

	bool removed;
};

