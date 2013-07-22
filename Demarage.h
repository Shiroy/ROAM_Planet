#pragma once
#include "Sphere.h"
#include <OGRE\OgreCamera.h>
#include <OGRE\OgreSceneManager.h>
#include <OGRE\OgreRenderWindow.h>
#include <OGRE\OgreViewport.h>
#include <OGRE\OgreRoot.h>
#include <OGRE\OgreWindowEventUtilities.h>
#include <OIS\OIS.h>

class Demarage : public Ogre::FrameListener, public Ogre::WindowEventListener
{
public:
	Demarage(void);
	~Demarage(void);

	void go();

	virtual bool frameRenderingQueued(const Ogre::FrameEvent &e);
	virtual void windowResized(Ogre::RenderWindow *wnd);
	virtual void windowClosed(Ogre::RenderWindow *wnd);

	Ogre::Camera *getCamera();
	Ogre::SceneManager *getSceneManager();
	Ogre::RenderWindow *getWindow();
	Ogre::Viewport *getViewport();

	void generateFace(Ogre::Radian theta, Ogre::Radian phi, Ogre::Real r, Ogre::ManualObject *obj);

private:
	void createViewport();
	void createWindow();
	void createSceneManager();
	void createCamera();
	void createScene();
	void startOIS();

	Ogre::Root *m_root;
	Ogre::SceneManager *m_sceneMgr;
	Ogre::RenderWindow *m_window;
	Ogre::Camera *m_camera;
	Ogre::Viewport *m_viewport;
	Ogre::SceneNode *m_node;

	OIS::InputManager *m_inputManager;

	OIS::Mouse *m_mouse;
	OIS::Keyboard *m_keyboard;

	Sphere *m_planet;

	bool updateKeyPressed;
	bool stopUpdate;
};

