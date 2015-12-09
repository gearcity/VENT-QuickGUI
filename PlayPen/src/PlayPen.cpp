#include "PlayPen.h"

#include "MainForm.h"

PlayPen::PlayPen() :
	mQuit(false)
{
	mOgreRoot = new Ogre::Root("", "");

	bool useOpenGL = 0;

#if defined _DEBUG
	mOgreRoot->loadPlugin("RenderSystem_GL_d.dll");
	mOgreRoot->loadPlugin("RenderSystem_Direct3D9_d.dll");
#else
	mOgreRoot->loadPlugin("RenderSystem_GL.dll");
	mOgreRoot->loadPlugin("RenderSystem_Direct3D9.dll");
#endif

	if(useOpenGL)
	{
		mOgreRoot->setRenderSystem((*(mOgreRoot->getAvailableRenderers()))[0]);
		mRenderSystem = mOgreRoot->getRenderSystem();

		// OpenGL Parameters

		mRenderSystem->setConfigOption( "Colour Depth", "32" );
		mRenderSystem->setConfigOption( "Display Frequency", "N/A" );
		mRenderSystem->setConfigOption( "FSAA", "0" );
		mRenderSystem->setConfigOption( "Full Screen", "No" );
		mRenderSystem->setConfigOption( "RTT Preferred Mode", "FBO" );
		mRenderSystem->setConfigOption( "VSync", "No" );
		mRenderSystem->setConfigOption( "Video Mode", "800 x 600" );
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
		mRenderSystem->setConfigOption( "Video Mode", "800 x 600 @ 32-bit colour" );
	}
	mRenderSystem->validateConfigOptions();

	QuickGUI::registerScriptReader();

	Ogre::ResourceGroupManager::getSingleton().addResourceLocation("qgui.core.zip", "Zip");

	Ogre::RenderWindow* w = mOgreRoot->initialise(true);

	mMainForm = new MainForm(this,mOgreRoot->getAutoCreatedWindow());

	_start();
}

PlayPen::~PlayPen()
{
	delete mMainForm;	

	delete mOgreRoot;
}

void PlayPen::_start()
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

void PlayPen::exit()
{
	mQuit = true;
}

bool PlayPen::shuttingDown()
{
	return mQuit;
}
