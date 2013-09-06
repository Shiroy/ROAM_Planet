#include <OGRE/Ogre.h>
#include <iostream>
#include "Demarage.h"
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#include <Windows.h>
#define WIN32_LEAN_AND_MEAN
#endif

int main(int argc, char *argv[])
{
	Demarage app;

	try
	{
		app.go();
	}
	catch(Ogre::Exception &e)
	{
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
		MessageBox(NULL, e.getFullDescription().c_str(), "Exception", MB_OK | MB_ICONERROR);
#else
		fprintf(stderr, "[Exception] : %s\n", e.getFullDescription().c_str());
#endif
	}

	return 0;
}
