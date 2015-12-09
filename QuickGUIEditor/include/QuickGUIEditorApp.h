#ifndef QUICKGUIEDITORAPP_H
#define QUICKGUIEDITORAPP_H

#include "QuickGUI.h"

namespace Ogre
{
	// forward declarations
	class Root;
	class RenderSystem;
}

namespace QuickGUIEditor
{
	// forward declarations
	class MainForm;

	class QuickGUIEditorApp
	{
	public:
		QuickGUIEditorApp();
		~QuickGUIEditorApp();

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
