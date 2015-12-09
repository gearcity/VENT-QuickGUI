#include "Window.h"

#include "OgreParticleSystemManager.h"
#include "OgreMeshManager.h"

Window::Window(PlayPen* e, Ogre::RenderWindow* w) :
	mSkinSetEditor(e),
	mRenderWindow(w)
{
	mOgreRoot = Ogre::Root::getSingletonPtr();
	mRenderSystem = mOgreRoot->getRenderSystem();

	Ogre::WindowEventUtilities::addWindowEventListener(mRenderWindow,this);

	mSceneManager = mOgreRoot->createSceneManager(Ogre::ST_GENERIC,w->getName() + ".SceneManager");
	mCamera = mSceneManager->createCamera(w->getName() + ".Camera");
	mViewport = mRenderWindow->addViewport(mCamera);
	mCamera->setAspectRatio(Ogre::Real(mViewport->getActualWidth()) / Ogre::Real(mViewport->getActualHeight()));

	_initializeOIS();

	Ogre::ResourceGroupManager::getSingletonPtr()->initialiseAllResourceGroups();

	QuickGUI::Root* guiRoot = new QuickGUI::Root();

	QuickGUI::SkinTypeManager::getSingleton().loadTypes();

	QuickGUI::GUIManagerDesc d;
	mGUIManager = QuickGUI::Root::getSingletonPtr()->createGUIManager(d);
}

Window::~Window()
{
	QuickGUI::Root::getSingletonPtr()->destroyGUIManager(mGUIManager);
	delete QuickGUI::Root::getSingletonPtr();

	OIS::InputManager::destroyInputSystem(mInputManager);

	mSceneManager->clearScene();
	mOgreRoot->destroySceneManager(mSceneManager);

	Ogre::WindowEventUtilities::removeWindowEventListener(mRenderWindow,this);

	mRenderSystem->destroyRenderTarget(mRenderWindow->getName());
}

void Window::_initializeOIS()
{
	OIS::ParamList pl;	
	size_t windowHnd = 0;
	std::ostringstream windowHndStr;

	mRenderWindow->getCustomAttribute("WINDOW",&windowHnd);
	windowHndStr << windowHnd;
	pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));

	//Default mode is foreground exclusive..but, we want to show mouse - so nonexclusive
	pl.insert(std::make_pair(std::string("w32_mouse"), std::string("DISCL_FOREGROUND" )));
	pl.insert(std::make_pair(std::string("w32_mouse"), std::string("DISCL_NONEXCLUSIVE")));

	mInputManager = OIS::InputManager::createInputSystem( pl );

	//Create all devices
	mKeyboard = static_cast<OIS::Keyboard*>(mInputManager->createInputObject( OIS::OISKeyboard, true ));
	mMouse = static_cast<OIS::Mouse*>(mInputManager->createInputObject( OIS::OISMouse, true ));

	mKeyboard->setEventCallback(this);
	mMouse->setEventCallback(this);

	//Set initial mouse clipping size
	mMouse->getMouseState().width = mRenderWindow->getWidth();
	mMouse->getMouseState().height = mRenderWindow->getHeight();
}

bool Window::keyPressed(const OIS::KeyEvent &arg)
{
	mGUIManager->injectChar(static_cast<Ogre::UTFString::unicode_char>(arg.text));
	mGUIManager->injectKeyDown(static_cast<QuickGUI::KeyCode>(arg.key));

	return true;
}

bool Window::keyReleased(const OIS::KeyEvent &arg)
{
	mGUIManager->injectKeyUp(static_cast<QuickGUI::KeyCode>(arg.key));

	return true;
}

bool Window::mouseMoved(const OIS::MouseEvent &arg)
{
	mGUIManager->injectMousePosition(arg.state.X.abs, arg.state.Y.abs);

	float z = arg.state.Z.rel;
	if(z != 0)
		mGUIManager->injectMouseWheelChange(z);

	return true;
}

bool Window::mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
	mGUIManager->injectMouseButtonDown(static_cast<QuickGUI::MouseButtonID>(id));

	return true;
}

bool Window::mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
	mGUIManager->injectMouseButtonUp(static_cast<QuickGUI::MouseButtonID>(id));

	return true;
}

void Window::update(float timeSinceLastFrame)
{
	mRenderWindow->setActive(true);

	mKeyboard->capture();
	mMouse->capture();
}

void Window::windowMoved(Ogre::RenderWindow* rw)
{
}

void Window::windowResized(Ogre::RenderWindow* rw)
{
	mMouse->getMouseState().width = rw->getWidth();
	mMouse->getMouseState().height = rw->getHeight();

	mCamera->setAspectRatio(Ogre::Real(mViewport->getActualWidth()) / Ogre::Real(mViewport->getActualHeight()));
	mGUIManager->getActiveSheet()->redraw();
	mGUIManager->notifyViewportDimensionsChanged();
}

void Window::windowClosed(Ogre::RenderWindow* rw)
{
}

void Window::windowFocusChange(Ogre::RenderWindow* rw)
{
}
