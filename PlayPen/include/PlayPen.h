#ifndef SKINSETEDITOR_H
#define SKINSETEDITOR_H

#include "OgreRenderSystem.h"
#include "OgreRoot.h"

// forward declarations
class MainForm;

class PlayPen
{
public:
	PlayPen();
	~PlayPen();

	void exit();

	bool shuttingDown();

protected:
	Ogre::Root*					mOgreRoot;
	Ogre::RenderSystem*			mRenderSystem;

	bool						mQuit;

	MainForm*					mMainForm;

private:
	void _initializeOIS();
	void _start();
};

#endif
