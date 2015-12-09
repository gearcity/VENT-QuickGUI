#include <Ogre.h>

#include "AnimationViewer.h"

#include "MainForm.h"

namespace AnimationViewer
{
	AnimationViewerApp::AnimationViewerApp() :
		mQuit(false)
	{
		mOgreRoot = new Ogre::Root("", "");
	#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
		bool useOpenGL = 0;
	#if defined _DEBUG
		if(useOpenGL)
			mOgreRoot->loadPlugin("RenderSystem_GL_d.dll");
		else
			mOgreRoot->loadPlugin("RenderSystem_Direct3D9_d.dll");
	#else
		if(useOpenGL)
			mOgreRoot->loadPlugin("RenderSystem_GL.dll");
		else
			mOgreRoot->loadPlugin("RenderSystem_Direct3D9.dll");
	#endif
	#elif OGRE_PLATFORM == OGRE_PLATFORM_LINUX
		bool useOpenGL = 1;
		mOgreRoot->loadPlugin("/usr/local/lib/OGRE/RenderSystem_GL.so");
	#endif

		if(useOpenGL)
		{
			// Ogre 1.6 uses this
			//mOgreRoot->setRenderSystem(mOgreRoot->getAvailableRenderers()->front());
			// Ogre Trunk (1.7) uses this
			mOgreRoot->setRenderSystem(mOgreRoot->getAvailableRenderers().front());
			mRenderSystem = mOgreRoot->getRenderSystem();
			// OpenGL Parameters

			mRenderSystem->setConfigOption( "Full Screen", "No" );
			mRenderSystem->setConfigOption( "Video Mode", "800 x 600" );
		}
		else
		{
			// Ogre 1.6 uses this
			//mOgreRoot->setRenderSystem(mOgreRoot->getAvailableRenderers()->front());
			// Ogre Trunk (1.7) uses this
			mOgreRoot->setRenderSystem(mOgreRoot->getAvailableRenderers().front());
			mRenderSystem = mOgreRoot->getRenderSystem();

			// DirectX Parameters

			mRenderSystem->setConfigOption( "Full Screen", "No" );
			mRenderSystem->setConfigOption( "Video Mode", "800 x 600 @ 32-bit colour" );
		}

		mRenderSystem->validateConfigOptions();

		QuickGUI::registerScriptReader();

		Ogre::ResourceGroupManager::getSingleton().addResourceLocation("qgui.core.zip", "Zip");
		Ogre::ResourceGroupManager::getSingleton().addResourceLocation(".", "FileSystem");

		Ogre::RenderWindow* w = mOgreRoot->initialise(true);

		mMainForm = new MainForm(this,mOgreRoot->getAutoCreatedWindow());

		_start();
	}

	AnimationViewerApp::~AnimationViewerApp()
	{
		delete mMainForm;

		delete mOgreRoot;
	}

	void AnimationViewerApp::_start()
	{
		Ogre::Timer timer;

		std::vector<Window*>::iterator it;

		while(!mQuit)
		{
			Ogre::WindowEventUtilities::messagePump();

			mOgreRoot->renderOneFrame();

			mMainForm->update(timer.getMicroseconds() / 1000.0);
			timer.reset();
		}

		//QuickGUI::Root::getSingleton().getGUIManager("GUIManager.0")->getActiveSheet()->saveToDisk(".\\resources\\DefaultSheet.sheet");
	}

	void AnimationViewerApp::exit()
	{
		mQuit = true;
	}

	bool AnimationViewerApp::shuttingDown()
	{
		return mQuit;
	}
}
