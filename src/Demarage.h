#pragma once
#include "Planet.h"
#include <OGRE/OgreCamera.h>
#include <OGRE/OgreSceneManager.h>
#include <OGRE/OgreRenderWindow.h>
#include <OGRE/OgreViewport.h>
#include <OGRE/OgreRoot.h>
#include <OGRE/OgreWindowEventUtilities.h>
#include <OIS/OIS.h>
#ifdef __linux__
#include <boost/timer.hpp>
#endif


class Demarage : public Ogre::FrameListener, public Ogre::WindowEventListener/*, public OIS::KeyListener, public OIS::MouseListener*/
{
public:
	Demarage(void);
	~Demarage(void);

	void go();

	virtual bool frameRenderingQueued(const Ogre::FrameEvent &e);
	virtual void windowResized(Ogre::RenderWindow *wnd);
	virtual void windowClosed(Ogre::RenderWindow *wnd);

	//virtual bool mouseMoved(const OIS::MouseEvent &e);
	//virtual bool keyPressed(const OIS::KeyEvent &e);
	//virtual bool keyReleased(const OIS::KeyEvent &e);

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
	Ogre::SceneNode *m_node, *m_node2;
	Ogre::SceneNode * m_lightNode;

	OIS::InputManager *m_inputManager;

	OIS::Mouse *m_mouse;
	OIS::Keyboard *m_keyboard;

    Planet *m_planet;

	bool updateKeyPressed;
	bool stopUpdate;

	int lastfpstick;
	int currenttick;

	int lastUpdateTick;
};

