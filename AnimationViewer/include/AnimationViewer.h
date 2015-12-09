#ifndef AnimationViewerApp_H
#define AnimationViewerApp_H

#include "QuickGUI.h"

namespace Ogre
{
	// forward declarations
	class Root;
	class RenderSystem;
}

namespace AnimationViewer
{
	// forward declarations
	class MainForm;

	class AnimationViewerApp
	{
	public:
		AnimationViewerApp();
		~AnimationViewerApp();

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
}

#endif
