#include "QuickGUIEditorApp.h"

#include "OgreRenderSystem.h"
#include "OgreRoot.h"

#include "MainForm.h"

namespace QuickGUIEditor
{
	QuickGUIEditorApp::QuickGUIEditorApp() :
		mQuit(false)
	{
		mOgreRoot = new Ogre::Root("", "");

#if OGRE_PLATFORM == PLATFORM_WIN32 || OGRE_PLATFORM == OGRE_PLATFORM_WIN32
		bool useOpenGL = 0;
    #if defined _DEBUG
        mOgreRoot->loadPlugin("RenderSystem_GL_d.dll");
        mOgreRoot->loadPlugin("RenderSystem_Direct3D9_d.dll");
    #else
        mOgreRoot->loadPlugin("RenderSystem_GL.dll");
        mOgreRoot->loadPlugin("RenderSystem_Direct3D9.dll");
    #endif
#else
        bool useOpenGL = 1;
        mOgreRoot->loadPlugin("/usr/local/lib/OGRE/RenderSystem_GL.so");
#endif

			if(useOpenGL)
		{
			mOgreRoot->setRenderSystem((*(mOgreRoot->getAvailableRenderers()))[0]);
			mRenderSystem = mOgreRoot->getRenderSystem();

			// OpenGL Parameters

			//mRenderSystem->setConfigOption( "Colour Depth", "32" );
			mRenderSystem->setConfigOption( "Display Frequency", "N/A" );
			mRenderSystem->setConfigOption( "FSAA", "0" );
			mRenderSystem->setConfigOption( "Full Screen", "No" );
			mRenderSystem->setConfigOption( "RTT Preferred Mode", "FBO" );
			mRenderSystem->setConfigOption( "VSync", "No" );
			mRenderSystem->setConfigOption( "Video Mode", "1024 x 768" );
		}
		else
		{
			mOgreRoot->setRenderSystem((*(mOgreRoot->getAvailableRenderers()))[1]);
			mRenderSystem = mOgreRoot->getRenderSystem();

			// DirectX Parameters

			mRenderSystem->setConfigOption( "Allow NVPerfHUD", "No" );
			mRenderSystem->setConfigOption( "Anti aliasing", "None" );
			mRenderSystem->setConfigOption( "Floating-point mode", "Fastest" );
			mRenderSystem->setConfigOption( "Full Screen", "No" );
			mRenderSystem->setConfigOption( "VSync", "No" );
			mRenderSystem->setConfigOption( "Video Mode", "1024 x 768 @ 32-bit colour" );
		}

		mRenderSystem->validateConfigOptions();

		QuickGUI::registerScriptReader();
#if OGRE_PLATFORM == PLATFORM_WIN32 || OGRE_PLATFORM == OGRE_PLATFORM_WIN32
		Ogre::ResourceGroupManager::getSingleton().addResourceLocation("user/sheets", "FileSystem");
		Ogre::ResourceGroupManager::getSingleton().addResourceLocation("user/skins", "FileSystem");
		Ogre::ResourceGroupManager::getSingleton().addResourceLocation("qgui.core.zip", "Zip");
#else
		Ogre::ResourceGroupManager::getSingleton().addResourceLocation("../user/sheets", "FileSystem");
		Ogre::ResourceGroupManager::getSingleton().addResourceLocation("../user/skins", "FileSystem");
		Ogre::ResourceGroupManager::getSingleton().addResourceLocation("../qgui.core.zip", "Zip");
#endif

		Ogre::RenderWindow* w = mOgreRoot->initialise(true);

		mMainForm = new MainForm(this,mOgreRoot->getAutoCreatedWindow());

		_start();
	}

	QuickGUIEditorApp::~QuickGUIEditorApp()
	{
		delete mMainForm;

		delete mOgreRoot;
	}

	void QuickGUIEditorApp::_start()
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
	}

	void QuickGUIEditorApp::exit()
	{
		mQuit = true;
	}

	bool QuickGUIEditorApp::shuttingDown()
	{
		return mQuit;
	}
}
