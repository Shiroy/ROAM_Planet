#include "Demarage.h"
#include "Sphere.h"
#include "Triangle.h"
#include <OGRE\Ogre.h>

Demarage::Demarage(void)
{
	m_root = 0;
	m_sceneMgr = 0;
	m_window = 0;
	m_camera = 0;
	m_viewport = 0;
	m_inputManager = NULL;
	m_mouse = 0;
	m_keyboard = 0;

	updateKeyPressed = false;
	stopUpdate = false;

	lastfpstick = 0;
	currenttick = 0;
}


Demarage::~Demarage(void)
{
	Ogre::WindowEventUtilities::removeWindowEventListener(m_window, this);
	windowClosed(m_window);
}

void Demarage::go()
{
	m_root = new Ogre::Root("plugins.cfg", "ogre.cfg", "Ogre.log");

	Ogre::ConfigFile cfg;
	cfg.load("ressource.cfg");

	Ogre::ConfigFile::SectionIterator sectionIterator = cfg.getSectionIterator();
	Ogre::String sectionName, archName, typeName;
	while(sectionIterator.hasMoreElements())
	{
		sectionName = sectionIterator.peekNextKey();
		Ogre::ConfigFile::SettingsMultiMap *settings = sectionIterator.getNext();
		Ogre::ConfigFile::SettingsMultiMap::iterator settingsIterator = settings->begin();
		for(; settingsIterator != settings->end(); ++settingsIterator)
		{
			typeName = settingsIterator->first;
			archName = settingsIterator->second;
			Ogre::ResourceGroupManager::getSingleton().addResourceLocation(archName, typeName, sectionName);
		}
	}

	if(!(m_root->restoreConfig() || m_root->showConfigDialog()))
		return;

	m_window = m_root->initialise(true, "Ogre ROAM test 1");

	Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);
	Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

	createSceneManager();
	createCamera();
	createViewport();
	startOIS();
	createScene();

	m_root->addFrameListener(this);

	m_root->startRendering();
}

void Demarage::createSceneManager()
{	
	m_sceneMgr = m_root->createSceneManager(Ogre::ST_EXTERIOR_REAL_FAR);
}

void Demarage::createCamera()
{
	m_camera = m_sceneMgr->createCamera("DefaultCamera");
	m_camera->setNearClipDistance(1);
	m_camera->setFarClipDistance(200000);
	m_camera->setPosition(Ogre::Vector3(0, 0, -1002));
	m_camera->lookAt(Ogre::Vector3(0, 0, 0));
	m_camera->setPolygonMode(Ogre::PM_WIREFRAME);
}

void Demarage::createViewport()
{
	m_viewport = m_window->addViewport(m_camera);
	m_camera->setAspectRatio(Ogre::Real(Ogre::Real(m_viewport->getActualWidth()) / Ogre::Real(m_viewport->getActualHeight())));
	m_viewport->setBackgroundColour(Ogre::ColourValue(0, 0, 0));
}
	
void Demarage::createScene()
{
	m_node = m_sceneMgr->getRootSceneNode()->createChildSceneNode("CubeNode");

	Ogre::ManualObject *obj = m_sceneMgr->createManualObject("cube");
	obj->setDynamic(true);
	m_planet = new Sphere(1000.0f, obj);
	
	m_node->attachObject(obj);
	//m_node->rotate(Ogre::Vector3(0, 1, 0), Ogre::Radian(-Ogre::Math::PI/4));
}

bool Demarage::frameRenderingQueued(const Ogre::FrameEvent &e)
{
	if(m_window->isClosed())
		return false;

	m_keyboard->capture();
	m_mouse->capture();

	currenttick = GetTickCount();
	/*if((currenttick - lastfpstick) > 1000)
	{
		std::cout << "FPS : " << m_window->getAverageFPS() << std::endl;
		lastfpstick = currenttick;
	}*/
	
	if(!stopUpdate)
		m_planet->updateMesh(m_node->getPosition() - m_camera->getPosition(), m_camera);

	//std::cout << "Distance : " <<(m_camera->getPosition() - m_node->getPosition()).length() << " (update : " << (stopUpdate == true ? "off" : "on") << ")" << std::endl;

	if(m_keyboard->isKeyDown(OIS::KC_ESCAPE))
		return false;

	Ogre::Real vitesse = 1000.0f;
	Ogre::Real vitesseRot = 0.01f;

	Ogre::Real mouvement = vitesse * e.timeSinceLastFrame;
	Ogre::Vector3 deplacement = Ogre::Vector3::ZERO;

	if(m_keyboard->isKeyDown(OIS::KC_W))
		deplacement.z -= mouvement;
	if(m_keyboard->isKeyDown(OIS::KC_S))
		deplacement.z += mouvement;
	if(m_keyboard->isKeyDown(OIS::KC_A))
		deplacement.x -= mouvement;
	if(m_keyboard->isKeyDown(OIS::KC_D))
		deplacement.x += mouvement;

	

	if(m_keyboard->isKeyDown(OIS::KC_U) && !updateKeyPressed)
	{
		stopUpdate = !stopUpdate;
		updateKeyPressed = true;
	}
	else
		updateKeyPressed = false;

	//Screenshot
	if(m_keyboard->isKeyDown(OIS::KC_F12))
		m_window->writeContentsToTimestampedFile("screen", ".jpg");

	//std::cout << "FPS : " << m_window->getAverageFPS() << std::endl;

	OIS::MouseState state = m_mouse->getMouseState();
	Ogre::Radian mRotationX = Ogre::Radian(Ogre::Real(-state.Y.rel * vitesseRot));
	Ogre::Radian mRotationY = Ogre::Radian(Ogre::Real(-state.X.rel * vitesseRot));

	//std::cout << "Mouvement : " << mouvement << " time : " << e.timeSinceLastFrame << std::endl;

	/*if(deplacement != Ogre::Vector3::ZERO)
	{
		std::cout << "X : " << deplacement.x << " Y : " << deplacement.y << " Z : " << deplacement.z << std::endl;
	}*/

	m_camera->yaw(mRotationY);
	m_camera->pitch(mRotationX);
	m_camera->moveRelative(deplacement);

	return true;
}

void Demarage::startOIS()
{
	OIS::ParamList plist;
	size_t wndHnd = 0;
	std::ostringstream wndHndStr;

	m_window->getCustomAttribute("WINDOW", &wndHnd);
	wndHndStr << wndHnd;
	plist.insert(std::make_pair(std::string("WINDOW"), wndHndStr.str()));

	m_inputManager = OIS::InputManager::createInputSystem(plist);

	m_mouse = static_cast<OIS::Mouse*>(m_inputManager->createInputObject(OIS::OISMouse, false));
	m_keyboard = static_cast<OIS::Keyboard*>(m_inputManager->createInputObject(OIS::OISKeyboard, false));

	Ogre::WindowEventUtilities::addWindowEventListener(m_window, this);
	windowResized(m_window);
}

void Demarage::windowResized(Ogre::RenderWindow *wnd)
{
	const OIS::MouseState &state = m_mouse->getMouseState();
	state.height = wnd->getHeight();
	state.width = wnd->getWidth();
}

void Demarage::windowClosed(Ogre::RenderWindow *wnd)
{
	if(wnd == m_window)
	{
		if(m_inputManager)
		{
			m_inputManager->destroyInputObject(m_mouse);
			m_inputManager->destroyInputObject(m_keyboard);
			OIS::InputManager::destroyInputSystem(m_inputManager);
			m_inputManager = NULL;
		}
	}
}
