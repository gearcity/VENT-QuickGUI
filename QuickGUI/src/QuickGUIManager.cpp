#include "QuickGUIManager.h"
#include "QuickGUISerialReader.h"
#include "QuickGUIRoot.h"
#include "QuickGUIFactoryManager.h"
#include "QuickGUIWidgetFactory.h"
#include "QuickGUISheet.h"
#include "QuickGUISheetManager.h"
#include "QuickGUITimerManager.h"
#include "QuickGUIContextMenu.h"

#include "OgreSceneManager.h"
#include "OgreViewport.h"
#include "OgreRenderQueue.h"
#include "OgreCamera.h"
#include "OgreTimer.h"

namespace QuickGUI
{
	GUIManagerDesc::GUIManagerDesc()
	{
		name = "";
		viewport = NULL;
		sceneManager = NULL;
		scrollLastClicked = false;
		queueID = Ogre::RENDER_QUEUE_OVERLAY;
		determineClickEvents = true;
		clickTime = 400;
		doubleClickTime = 800;
		tripleClickTime = 800;
		supportAutoRepeat = true;
		timeToAutoRepeat = 0.6;
		autoRepeatInterval = 0.17;
		injectNumPadKeyCodesAsUnicodeChars = true;
		processNumPadKeyCodes = true;

		// by default, we support codepoints 9, and 32-166.
		supportedCodePoints.insert(9);
		for(Ogre::UTFString::code_point i = 32; i < 167; ++i)
			supportedCodePoints.insert(i);
	}

	GUIManager::GUIManager(GUIManagerDesc& d) :
		mAutoRepeatEnableTimer(NULL),
		mAutoRepeatInjectionTimer(NULL),
		mDraggingWidget(false),
		mActiveSheet(0),
		mButtonMask(0),
		mKeyDownEvent(false),
		mWidgetUnderMouseCursor(0),
		mLastClickedWidget(0),
		mDownOnBorder(false),
		mKeyModifiers(0),
		mPreviousBorder(BORDER_NONE),
		mResizableBorder(BORDER_NONE),
		mViewportWidth(0),
		mViewportHeight(0)
	{
		mGUIManagerDesc.name = d.name;
		mGUIManagerDesc.clickTime = d.clickTime;
		mGUIManagerDesc.doubleClickTime = d.doubleClickTime;
		mGUIManagerDesc.tripleClickTime = d.tripleClickTime;
		mGUIManagerDesc.determineClickEvents = d.determineClickEvents;
		mGUIManagerDesc.scrollLastClicked = d.scrollLastClicked;

		mBrush = Brush::getSingletonPtr();

		// If the scenemanager argument is NULL, try to use the first available scene manager
		if(d.sceneManager == NULL)
		{
                        auto it = Ogre::Root::getSingleton().getSceneManagers();
                        if(!it.empty())
                                setSceneManager((*it.begin()).second);
		}
		else
			setSceneManager(d.sceneManager);

		// If the viewport argument is NULL, use the first available viewport, from the scene manager chosen above.
		if(d.viewport == NULL)
		{
			if(mGUIManagerDesc.sceneManager != NULL)
			{
				Ogre::SceneManager::CameraIterator it = mGUIManagerDesc.sceneManager->getCameraIterator();
				if(it.hasMoreElements())
                    setViewport(it.getNext()->getViewport());
			}
		}
		else
			setViewport(d.viewport);

		setSupportedCodePoints(d.supportedCodePoints);
		setInjectNumPadKeyCodesAsUnicodeChars(d.injectNumPadKeyCodesAsUnicodeChars);
		setProcessNumPadKeyCodes(d.processNumPadKeyCodes);

		d.mouseCursorDesc.guiManager = this;
		mMouseCursor = OGRE_NEW_T(MouseCursor,Ogre::MEMCATEGORY_GENERAL)(d.mouseCursorDesc);
		_clearMouseTrackingData();

		mTimer = OGRE_NEW Ogre::Timer();
		
		TimerDesc td;
		td.repeat = false;
		td.timePeriod = 3;
		mHoverTimer = TimerManager::getSingleton().createTimer(td);
		mHoverTimer->setCallback(&GUIManager::hoverTimerCallback,this);

		setRenderQueueID(d.queueID);

		enableAutoInjection(d.supportAutoRepeat);
		setAutoRepeatInterval(d.autoRepeatInterval);
		setTimeToAutoRepeat(d.timeToAutoRepeat);
	}

	GUIManager::~GUIManager()
	{
		// Clear out freelist
		while(!mFreeList.empty())
		{
			FactoryManager::getSingleton().getWidgetFactory()->destroyInstance(mFreeList.front());
			mFreeList.pop_front();
		}

		TimerManager::getSingleton().destroyTimer(mHoverTimer);

		OGRE_DELETE mTimer;

		OGRE_DELETE_T(mMouseCursor,MouseCursor,Ogre::MEMCATEGORY_GENERAL);
	}

	void GUIManager::_clearMouseTrackingData()
	{
		for(int i = 0; i < NUM_MOUSE_BUTTONS; ++i)
		{
			mMouseButtonDown[i] = NULL;
		}
	}

	void GUIManager::autoRepeatEnableCallback()
	{
		if(mAutoRepeatInjectionTimer != NULL)
		{
			mAutoRepeatInjectionTimer->reset();
			mAutoRepeatInjectionTimer->start();
		}
	}

	void GUIManager::autoRepeatInjectionCallback()
	{
		if(mKeyDownEvent)
		{
			injectChar(mLastKeyInjection.codepoint);
			injectKeyDown(mLastKeyInjection.scancode);
		}
		else
		{
			injectMouseButtonDown(mLastMouseButtonInjection.button);
		}
	}

	void GUIManager::checkIfCursorOverResizableBorder(Point position)
	{
		// Change cursor if we're over a resizable widget's border, or restore cursor if we have left resizable widget's border
		if(mWidgetUnderMouseCursor != NULL)
		{
			mPreviousBorder = mResizableBorder;
			mResizableBorder = mWidgetUnderMouseCursor->getBorderSide(position);

			switch(mResizableBorder)
			{
				case BORDER_LEFT:
					if(mWidgetUnderMouseCursor->getResizeFromLeft())
						mMouseCursor->_setSkinType("hresize");
					else mResizableBorder = BORDER_NONE;
					break;
				case BORDER_RIGHT:
					if(mWidgetUnderMouseCursor->getResizeFromRight())
						mMouseCursor->_setSkinType("hresize");
					else mResizableBorder = BORDER_NONE;
					break;
				case BORDER_TOP:
					if(mWidgetUnderMouseCursor->getResizeFromTop())
						mMouseCursor->_setSkinType("vresize");
					else mResizableBorder = BORDER_NONE;
					break;
				case BORDER_BOTTOM:
					if(mWidgetUnderMouseCursor->getResizeFromBottom())
						mMouseCursor->_setSkinType("vresize");
					else mResizableBorder = BORDER_NONE;
					break;
				case BORDER_TOP_LEFT:
					if(mWidgetUnderMouseCursor->getResizeFromLeft() && mWidgetUnderMouseCursor->getResizeFromTop())
						mMouseCursor->_setSkinType("diag1resize");
					else mResizableBorder = BORDER_NONE;
					break;
				case BORDER_BOTTOM_RIGHT:
					if(mWidgetUnderMouseCursor->getResizeFromRight() && mWidgetUnderMouseCursor->getResizeFromBottom())
						mMouseCursor->_setSkinType("diag1resize");
					else mResizableBorder = BORDER_NONE;
					break;
				case BORDER_TOP_RIGHT:
					if(mWidgetUnderMouseCursor->getResizeFromRight() && mWidgetUnderMouseCursor->getResizeFromTop())
						mMouseCursor->_setSkinType("diag2resize");
					else mResizableBorder = BORDER_NONE;
					break;
				case BORDER_BOTTOM_LEFT:
					if(mWidgetUnderMouseCursor->getResizeFromLeft() && mWidgetUnderMouseCursor->getResizeFromBottom())
						mMouseCursor->_setSkinType("diag2resize");
					else mResizableBorder = BORDER_NONE;
					break;
				case BORDER_NONE:
					// Revert cursor if we have just moved off a resizable border
					if(mPreviousBorder != BORDER_NONE)
						mMouseCursor->_setSkinType(mMouseCursor->mDefaultSkinType);
					break;
			}
		}
	}

	void GUIManager::clearWidgetUnderMouseCursorReference()
	{
		mWidgetUnderMouseCursor = NULL;

		// Make sure cursor is the default normal one
		mMouseCursor->_setSkinType(mMouseCursor->mDefaultSkinType);
	}

	void GUIManager::clearLastClickedWidgetReference()
	{
		mLastClickedWidget = NULL;
	}

	void GUIManager::draw()
	{
		SheetManager::getSingleton().cleanup();

		// Clear out freelist
		while(!mFreeList.empty())
		{
			FactoryManager::getSingleton().getWidgetFactory()->destroyInstance(mFreeList.front());
			mFreeList.pop_front();
		}

		mBrush->updateSceneManager(mGUIManagerDesc.sceneManager);
		mBrush->updateViewport(mGUIManagerDesc.viewport);
		mBrush->setRenderTarget(mGUIManagerDesc.viewport);

		mBrush->prepareToDraw();

		if(mActiveSheet != NULL)
		{
			mActiveSheet->cleanupWidgets();
			mActiveSheet->draw();
		}

		mMouseCursor->draw();

		mBrush->restore();
	}

	void GUIManager::enableAutoInjection(bool enable)
	{
		mGUIManagerDesc.supportAutoRepeat = enable;

		if(!enable)
		{
			TimerManager::getSingleton().destroyTimer(mAutoRepeatEnableTimer);
			mAutoRepeatEnableTimer = NULL;

			TimerManager::getSingleton().destroyTimer(mAutoRepeatInjectionTimer);
			mAutoRepeatInjectionTimer = NULL;
		}
		else
		{
			if(mAutoRepeatEnableTimer == NULL)
			{
				TimerDesc td;
				td.repeat = false;
				td.timePeriod = 0.6;
				mAutoRepeatEnableTimer = TimerManager::getSingleton().createTimer(td);
				mAutoRepeatEnableTimer->setCallback(&GUIManager::autoRepeatEnableCallback,this);
			}

			if(mAutoRepeatInjectionTimer == NULL)
			{
				TimerDesc td;
				td.repeat = true;
				td.timePeriod = 0.17;
				mAutoRepeatInjectionTimer = TimerManager::getSingleton().createTimer(td);
				mAutoRepeatInjectionTimer->setCallback(&GUIManager::autoRepeatInjectionCallback,this);
			}
		}
	}

	Sheet* GUIManager::getActiveSheet()
	{
		return mActiveSheet;
	}

	bool GUIManager::getInjectNumPadKeyCodesAsUnicodeChars()
	{
		return mGUIManagerDesc.injectNumPadKeyCodesAsUnicodeChars;
	}

	bool GUIManager::getProcessNumPadKeyCodes()
	{
		return mGUIManagerDesc.processNumPadKeyCodes;
	}

	Widget* GUIManager::getLastClickedWidget()
	{			
			return mLastClickedWidget;
	}

	MouseCursor* GUIManager::getMouseCursor()
	{
		return mMouseCursor;
	}

	Ogre::String GUIManager::getName()
	{
		return mGUIManagerDesc.name;
	}

	bool GUIManager::getNumLock()
	{
		return ((mKeyModifiers & NUMLOCK) != 0);
	}

	bool GUIManager::getScrollLastClicked()
	{
		return mGUIManagerDesc.scrollLastClicked;
	}

	Widget* GUIManager::getWidgetUnderMouseCursor()
	{
		return mWidgetUnderMouseCursor;
	}

	Ogre::Viewport* GUIManager::getViewport()
	{
		return mGUIManagerDesc.viewport;
	}

	bool GUIManager::injectChar(Ogre::UTFString::unicode_char c)
	{
		// do nothing if no active sheet is in use
		if(mActiveSheet == NULL)
			return false;

		// We store the codepoint regardless of whether its supported, for the auto repeat functionality to work properly.
		// Otherwise the last known codepoint will be the last *supported* codepoint, in which case holding the Left arrow key
		// could result in a stream of 'a's to be entered in the TextBox, as an example.
		mLastKeyInjection.codepoint = c;

		if(!isSupportedCodePoint(c))
			return false;

		Widget* w = mActiveSheet->getKeyboardListener();

		if((w == NULL) || !(w->getEnabled()))
			return false;

		// Update the structure holding our info about the last key injected, and use it to fire the event.
		mKeyDownEvent = true;
		mLastKeyInjection.widget = w;
		mLastKeyInjection.keyModifiers = mKeyModifiers;
		mLastKeyInjection.keyMask = mButtonMask;
		/*if((mAutoRepeatInjectionTimer != NULL) && (mAutoRepeatInjectionTimer->isUpdating()))
			mLastKeyInjection.autoRepeat = true;
		else
			mLastKeyInjection.autoRepeat = false;
*/
		w->fireWidgetEvent(WIDGET_EVENT_CHARACTER_KEY,mLastKeyInjection);

		// Enable Auto Repeat, which will begin after some time
	/*	if((mAutoRepeatEnableTimer != NULL) && (mAutoRepeatInjectionTimer != NULL) && (!mAutoRepeatInjectionTimer->isUpdating()))
		{
			mAutoRepeatEnableTimer->reset();
			mAutoRepeatEnableTimer->start();
		}
*/
		return true;
	}

	bool GUIManager::injectKeyDown(const KeyCode& kc)
	{
		KeyCode c = kc;
		if(mGUIManagerDesc.processNumPadKeyCodes)
			c = processNumPadKeyCode(kc);

		// do nothing if no active sheet is in use
		if(mActiveSheet == NULL)
			return false;

		// Turn on modifier
		if( (c == KC_LCONTROL) || (c == KC_RCONTROL) )
		{
			mKeyModifiers |= CTRL;
			return false;
		}
		else if( (c == KC_LSHIFT) || (c == KC_RSHIFT) )
		{
			mKeyModifiers |= SHIFT;
			return false;
		}
		else if( (c == KC_LMENU) || (c == KC_RMENU) )
		{
			mKeyModifiers |= ALT;
			return false;
		}
		else if( (c == KC_NUMLOCK) )
		{
			// Toggle NumLock
			setNumLock(!getNumLock());
			return false;
		}

		// OIS does not interpret NumPad keys correctly.  Translate NumPad KeyCodes
		// to unicode characters and inject them, if GUIManager has been configured to do so.
		if(mGUIManagerDesc.injectNumPadKeyCodesAsUnicodeChars)
			translateNumPadKeyCodesToUnicodeChars(c);

		Widget* w = mActiveSheet->getKeyboardListener();

		if((w == NULL) || !(w->getEnabled()))
			return false;

		// Update the structure holding our info about the last key injected, and use it to fire the event.
		mKeyDownEvent = true;
		mLastKeyInjection.widget = w;
		mLastKeyInjection.scancode = c;
		mLastKeyInjection.keyMask = mButtonMask;
		mLastKeyInjection.keyModifiers = mKeyModifiers;
	/*	if((mAutoRepeatInjectionTimer != NULL) && (mAutoRepeatInjectionTimer->isUpdating()))
			mLastKeyInjection.autoRepeat = true;
		else
			mLastKeyInjection.autoRepeat = false;
*/
		w->fireWidgetEvent(WIDGET_EVENT_KEY_DOWN,mLastKeyInjection);

		// Enable Auto Repeat, which will begin after some time
		/*if((mAutoRepeatEnableTimer != NULL) && (mAutoRepeatInjectionTimer != NULL) && (!mAutoRepeatInjectionTimer->isUpdating()))
		{
			mAutoRepeatEnableTimer->reset();
			mAutoRepeatEnableTimer->start();
		}*/

		return true;
	}

	bool GUIManager::injectKeyUp(const KeyCode& kc)
	{
		if(mAutoRepeatEnableTimer != NULL)
			mAutoRepeatEnableTimer->stop();
		if(mAutoRepeatInjectionTimer != NULL)
			mAutoRepeatInjectionTimer->stop();

		// do nothing if no active sheet is in use
		if(mActiveSheet == NULL)
			return false;

		//Turn off modifier
		if( (kc == KC_LCONTROL) || (kc == KC_RCONTROL) )
			mKeyModifiers &= ~CTRL;
		else if( (kc == KC_LSHIFT) || (kc == KC_RSHIFT) )
			mKeyModifiers &= ~SHIFT;
		else if( (kc == KC_LMENU) || (kc == KC_RMENU) )
			mKeyModifiers &= ~ALT;

		Widget* w = mActiveSheet->getKeyboardListener();

		if((w == NULL) || !(w->getEnabled()))
			return false;

		KeyEventArgs args(w);
		args.scancode = kc;
		args.keyModifiers = mKeyModifiers;

		w->fireWidgetEvent(WIDGET_EVENT_KEY_UP,args);

		return true;
	}

	bool GUIManager::injectMouseButtonDown(const MouseButtonID& button)
	{
		// do nothing if no active sheet is in use
		if(mActiveSheet == NULL)
			return false;

		// If the mouse is disabled, we do not accept this injection of input
		if( !mMouseCursor->getEnabled() ) 
			return false;

		// Users can manually inject single/double/triple click input, or have it detected
		// from mouse button up/down injections.
		if(mGUIManagerDesc.determineClickEvents)
		{
			// Record time of MouseDown, for single/double/triple click determination
			mTimeOfButtonDown[button] = mTimer->getMilliseconds();

			// Check if time since last double click is within triple click time
			if((mTimeOfButtonDown[button] - mTimeOfDoubleClick[button]) <= mGUIManagerDesc.tripleClickTime)
			{
				// We have passed the criteria for a triple click injection, but we have to 
				// make sure the mouse button is going down on the same widget for all clicks.
				if(mMouseButtonDown[button] == mWidgetUnderMouseCursor)
					return injectMouseTripleClick(button);
			}
			// Check if time since last click is within double click time
			if((mTimeOfButtonDown[button] - mTimeOfClick[button]) <= mGUIManagerDesc.doubleClickTime)
			{
				// We have passed the criteria for a double click injection, but we have to 
				// make sure the mouse button is going down on the same widget for both clicks.
				if(mMouseButtonDown[button] == mWidgetUnderMouseCursor)
					return injectMouseDoubleClick(button);
			}
		}

		// If we make it here, a simple mouse button down has occurred.

		// Modify the button mask
		mButtonMask |= (1 << button);

		// Record that the mouse button went down on this widget		
			mMouseButtonDown[button] = mWidgetUnderMouseCursor;
			mLastClickedWidget = mWidgetUnderMouseCursor;
	

		// Check if Sheet's keyboard listener (widget receiving keyboard events) needs to be updated.
		if((mLastClickedWidget != NULL) && (mLastClickedWidget->getConsumeKeyboardEvents()))
			mActiveSheet->setKeyboardListener(mLastClickedWidget);

		// Update the structure holding our info about the last mouse button down injected, and use it to fire the event.
		mKeyDownEvent = false;
		mLastMouseButtonInjection.widget = mWidgetUnderMouseCursor;
		mLastMouseButtonInjection.position = mMouseCursor->getPosition();
		mLastMouseButtonInjection.button = button;
		mLastMouseButtonInjection.buttonMask = mButtonMask;
		mLastMouseButtonInjection.keyModifiers = mKeyModifiers;
//		mLastMouseButtonInjection.eventType = MOUSE_EVENT_BUTTON_DOWN;
		if((mAutoRepeatInjectionTimer != NULL) && (mAutoRepeatInjectionTimer->isUpdating()))
			mLastMouseButtonInjection.autoRepeat = true;
		else
			mLastMouseButtonInjection.autoRepeat = false;

		// Get the Window under the mouse cursor.  If it is not the Sheet, but a child Window,
		// make sure it has focus. (bring to front)
		Window* win = mActiveSheet->findWindowAtPoint(mLastMouseButtonInjection.position);
		if(win != mActiveSheet->getWindowInFocus())
			// FOCUS_GAINED and FOCUS_LOST events will be fired if appropriate.
			mActiveSheet->focusWindow(win);

		if(mWidgetUnderMouseCursor != NULL)
		{
			if(button == MB_Left)
			{
				mWidgetUnderMouseCursor->setGrabbed(true);

				// If Widget is grabable, change cursor
				if(mWidgetUnderMouseCursor->getDragable())
					mMouseCursor->_setSkinType("grabbed");
			}

			// Fire EVENT_MOUSE_BUTTON_DOWN event to the widget in focus
			mWidgetUnderMouseCursor->fireWidgetEvent(WIDGET_EVENT_MOUSE_BUTTON_DOWN,mLastMouseButtonInjection);

			// If Widget isn't draggable, enable Auto Repeat, which will begin after some time
			if( (!mWidgetUnderMouseCursor->getDragable()) && (mAutoRepeatEnableTimer != NULL) &&
				(mAutoRepeatInjectionTimer != NULL) && (!mAutoRepeatInjectionTimer->isUpdating()))
			{
				mAutoRepeatEnableTimer->reset();
				mAutoRepeatEnableTimer->start();
			}

			// Record mouse down on border only if the widget supports resizing for that border, and LMB is down
			mDownOnBorder = false;
			if((win != NULL) && (button == MB_Left))
			{
				switch(mWidgetUnderMouseCursor->getBorderSide(mMouseCursor->getPosition() - win->getPosition()))
				{
				case BORDER_LEFT:
					if(mWidgetUnderMouseCursor->getResizeFromLeft())
						mDownOnBorder = true;
					break;
				case BORDER_RIGHT:
					if(mWidgetUnderMouseCursor->getResizeFromRight())
						mDownOnBorder = true;
					break;
				case BORDER_TOP:
					if(mWidgetUnderMouseCursor->getResizeFromTop())
						mDownOnBorder = true;
					break;
				case BORDER_BOTTOM:
					if(mWidgetUnderMouseCursor->getResizeFromBottom())
						mDownOnBorder = true;
					break;
				case BORDER_TOP_LEFT:
					if(mWidgetUnderMouseCursor->getResizeFromLeft() && mWidgetUnderMouseCursor->getResizeFromTop())
						mDownOnBorder = true;
					break;
				case BORDER_BOTTOM_RIGHT:
					if(mWidgetUnderMouseCursor->getResizeFromRight() && mWidgetUnderMouseCursor->getResizeFromBottom())
						mDownOnBorder = true;
					break;
				case BORDER_TOP_RIGHT:
					if(mWidgetUnderMouseCursor->getResizeFromRight() && mWidgetUnderMouseCursor->getResizeFromTop())
						mDownOnBorder = true;
					break;
				case BORDER_BOTTOM_LEFT:
					if(mWidgetUnderMouseCursor->getResizeFromLeft() && mWidgetUnderMouseCursor->getResizeFromBottom())
						mDownOnBorder = true;
					break;
				}
			}
		}

		return (mWidgetUnderMouseCursor != NULL);
	}

	bool GUIManager::injectMouseButtonUp(const MouseButtonID& button)
	{
		if(mAutoRepeatEnableTimer != NULL)
			mAutoRepeatEnableTimer->stop();
		if(mAutoRepeatInjectionTimer != NULL)
			mAutoRepeatInjectionTimer->stop();

		// do nothing if no active sheet is in use
		if(mActiveSheet == NULL)
			return false;

		// If the mouse is disabled, we do not accept this injection of input
		if( !mMouseCursor->getEnabled() ) 
			return false;

		// Modify the button mask
		mButtonMask &= !(1 << button);

		// If the mouse button goes up and is not over the same widget
		// the mouse button went down on, disregard this injection.
		if( mMouseButtonDown[button] != mWidgetUnderMouseCursor )
			return (mWidgetUnderMouseCursor != NULL);

		// after this point, we know that the user had mouse button down on this widget, and is now doing mouse button up on the same widget.

		// Create MouseEventArgs, for use with any fired events
		MouseEventArgs args(mWidgetUnderMouseCursor);
		args.position = mMouseCursor->getPosition();
		args.button = button;
		args.buttonMask = mButtonMask;
		args.keyModifiers = mKeyModifiers;
//		args.eventType = MOUSE_EVENT_BUTTON_UP;

		if(args.widget != NULL)
		{
			// Check if a widget is currently being dragged.
			if(args.widget->getGrabbed())
			{
				args.widget->setGrabbed(false);

				if(args.widget->getDragable())
				{
					// Set cursor to grabable
					mMouseCursor->_setSkinType("grabable");
				}
				else
				{
					// Restore cursor to default
					mMouseCursor->_setSkinType(mMouseCursor->mDefaultSkinType);
				}

				// We do not want the widget to receive an EVENT_MOUSE_BUTTON_UP event if we are dropping
				// the widget.  Think of Diablo II, dragging a potion to your belt.  If we sent the mouse
				// button up event the potion would be drank as soon as you dropped it into the belt.
				if(args.widget->getBeingDragged())
				{
					args.widget->fireWidgetEvent(WIDGET_EVENT_DROPPED,args);
					return (mWidgetUnderMouseCursor != NULL);
				}
			}

			// Fire EVENT_MOUSE_BUTTON_UP event
			args.widget->fireWidgetEvent(WIDGET_EVENT_MOUSE_BUTTON_UP,args);

			// Show context menu
			if((mActiveSheet != NULL) && (button == MB_Right) && (args.widget->getContextMenuName() != ""))
				mActiveSheet->getContextMenu(args.widget->getContextMenuName())->show(args.position);

			// Users can manually inject single/double/triple click input, or have it detected
			// from mouse button up/down injections.
			if(mGUIManagerDesc.determineClickEvents)
			{
				
				if((mTimer->getMilliseconds() - mTimeOfButtonDown[button]) <= mGUIManagerDesc.clickTime)
				{
					injectMouseClick(button);
				}
			}
		}

		return (mWidgetUnderMouseCursor != NULL);
	}

	bool GUIManager::injectMouseClick(const MouseButtonID& button)
	{
		// do nothing if no active sheet is in use
		if(mActiveSheet == NULL)
			return false;

		// If the mouse is disabled, we do not accept this injection of input
		if( !mMouseCursor->getEnabled() ) 
			return false;

			mLastClickedWidget = mWidgetUnderMouseCursor;

		if(mWidgetUnderMouseCursor->hasAnyEventHandlersForThisType(WIDGET_EVENT_MOUSE_CLICK) || mWidgetUnderMouseCursor->hasAnyEventHandlersForThisType(WIDGET_EVENT_MOUSE_CLICK_DOUBLE) || mWidgetUnderMouseCursor->hasAnyEventHandlersForThisType(WIDGET_EVENT_MOUSE_CLICK_TRIPLE) )
		{
		}
		else
		{
			return false;
		}

		// Modify the button mask
		mButtonMask &= !(1 << button);

		// Record the time the click occurred. Useful for generating double clicks.
		mTimeOfClick[button] = mTimer->getMilliseconds();

		// If the mouse button goes up and is not over the same widget
		// the mouse button went down on, disregard this injection.
		if( mMouseButtonDown[button] != mWidgetUnderMouseCursor )
			return (mWidgetUnderMouseCursor != NULL);

	

		// Create MouseEventArgs, for use with any fired events
		MouseEventArgs args(mWidgetUnderMouseCursor);
		args.button = button;
		args.position = mMouseCursor->getPosition();
		args.buttonMask = mButtonMask;
		args.keyModifiers = mKeyModifiers;
		//args.eventType = MOUSE_EVENT_BUTTON_CLICK;

		if(args.widget != NULL)
			args.widget->fireWidgetEvent(WIDGET_EVENT_MOUSE_CLICK,args);

		return (mWidgetUnderMouseCursor != NULL);
		
	}

	bool GUIManager::injectMouseDoubleClick(const MouseButtonID& button)
	{
	// do nothing if no active sheet is in use
		if(mActiveSheet == NULL)
			return false;

		// If the mouse is disabled, we do not accept this injection of input
		if( !mMouseCursor->getEnabled() ) 
			return false;

		mLastClickedWidget = mWidgetUnderMouseCursor;

		// Modify the button mask
		mButtonMask &= !(1 << button);

		// Record the time the click occurred. Useful for generating triple clicks.
		mTimeOfDoubleClick[button] = mTimer->getMilliseconds();

		// If the mouse button goes up and is not over the same widget
		// the mouse button went down on, disregard this injection.
		if( mMouseButtonDown[button] != mWidgetUnderMouseCursor )
			return (mWidgetUnderMouseCursor != NULL);

		

		// Create MouseEventArgs, for use with any fired events
		MouseEventArgs args(mWidgetUnderMouseCursor);
		args.button = button;
		args.position = mMouseCursor->getPosition();
		args.buttonMask = mButtonMask;
		args.keyModifiers = mKeyModifiers;
		//args.eventType = MOUSE_EVENT_BUTTON_DOUBLE_CLICK;

		if(args.widget != NULL)
			args.widget->fireWidgetEvent(WIDGET_EVENT_MOUSE_CLICK_DOUBLE,args);

		return (mWidgetUnderMouseCursor != NULL);
		
	}

	bool GUIManager::injectMouseTripleClick(const MouseButtonID& button)
	{
		// do nothing if no active sheet is in use
		if(mActiveSheet == NULL)
			return false;

		// If the mouse is disabled, we do not accept this injection of input
		if( !mMouseCursor->getEnabled() ) 
			return false;

		mLastClickedWidget = mWidgetUnderMouseCursor;

		// Modify the button mask
		mButtonMask &= !(1 << button);

		// If the mouse button goes up and is not over the same widget
		// the mouse button went down on, disregard this injection.
		if( mMouseButtonDown[button] != mWidgetUnderMouseCursor )
			return (mWidgetUnderMouseCursor != NULL);

		

		// Create MouseEventArgs, for use with any fired events
		MouseEventArgs args(mWidgetUnderMouseCursor);
		args.button = button;
		args.position = mMouseCursor->getPosition();
		args.buttonMask = mButtonMask;
		args.keyModifiers = mKeyModifiers;

		if(args.widget != NULL)
			args.widget->fireWidgetEvent(WIDGET_EVENT_MOUSE_CLICK_TRIPLE,args);

		return (mWidgetUnderMouseCursor != NULL);
	}

	

	bool GUIManager::injectMouseMove(const int& xPixelOffset, const int& yPixelOffset)
	{
		// do nothing if no active sheet is in use
		if(mActiveSheet == NULL)
			return false;

		// If the mouse is disabled, we do not accept this injection of input
		if( !mMouseCursor->getEnabled() ) 
			return false;

		// Restart the Hover Timer whenever the mouse moves.
		mHoverTimer->start();

		// Get the widget the cursor is over.
		Window* win = mActiveSheet->findWindowAtPoint(mMouseCursor->getPosition());
		Widget* w = win->findWidgetAtPoint(mMouseCursor->getPosition() - win->getPosition());

		// See if we should be dragging or resizing a widget.
		if((mWidgetUnderMouseCursor != NULL) && (mWidgetUnderMouseCursor->getGrabbed()))
		{
			// Check resizing first
			if((mResizableBorder != BORDER_NONE) && mDownOnBorder)
			{
				mWidgetUnderMouseCursor->resize(mResizableBorder,mMouseCursor->getPosition());

				return (mWidgetUnderMouseCursor != NULL);
			}
			// Check dragging
			else if(mWidgetUnderMouseCursor->getDragable())
			{
				// Dragging, which uses move function, works with pixel values (uninfluenced by parent dimensions!)
				mWidgetUnderMouseCursor->drag(xPixelOffset,yPixelOffset);

				return (mWidgetUnderMouseCursor != NULL);
			}
		}

		// Ignore disabled widgets
		if((w != NULL) && !(w->getEnabled()))
			return (mWidgetUnderMouseCursor != NULL);

		// Create MouseEventArgs, for use with any fired events
		MouseEventArgs args(w);
		args.position = mMouseCursor->getPosition();
		args.moveDelta.x = xPixelOffset;
		args.moveDelta.y = yPixelOffset;
		args.buttonMask = mButtonMask;
		args.keyModifiers = mKeyModifiers;
//		args.eventType = MOUSE_EVENT_MOVE;

		// Create a boolean to track whether or not this injection caused any significant changes.
		bool changesMade = false;

		// The Widget underneath the mouse cursor has changed.
		if( mWidgetUnderMouseCursor != w )
		{
			if(mWidgetUnderMouseCursor != NULL)
			{
				changesMade |= mWidgetUnderMouseCursor->fireWidgetEvent(WIDGET_EVENT_MOUSE_LEAVE,args);
				// Restore cursor to default
				mMouseCursor->_setSkinType(mMouseCursor->mDefaultSkinType);
			}

			// Update pointer
			mWidgetUnderMouseCursor = w;

			if(mWidgetUnderMouseCursor != NULL)
			{
				changesMade |= mWidgetUnderMouseCursor->fireWidgetEvent(WIDGET_EVENT_MOUSE_ENTER,args);

				// Set the Timer to match this widgets hover time.
				mHoverTimer->setTimePeriod(mWidgetUnderMouseCursor->getHoverTime());
			}
		}

		// Notify the widget the cursor has moved.
		if(mWidgetUnderMouseCursor != NULL)
		{
			changesMade |= mWidgetUnderMouseCursor->fireWidgetEvent(WIDGET_EVENT_MOUSE_MOVE,args);

			// If Widget is grabable, change cursor
			if(mWidgetUnderMouseCursor->getDragable())
				mMouseCursor->_setSkinType("grabable");
			else
				mMouseCursor->_setSkinType(mMouseCursor->mDefaultSkinType);
		}

		// Check if cursor needs to change
		checkIfCursorOverResizableBorder(mMouseCursor->getPosition() - win->getPosition());

		return (mWidgetUnderMouseCursor != NULL);
	}

	bool GUIManager::injectMousePosition(const int& xPixelPosition, const int& yPixelPosition)
	{
		// do nothing if no active sheet is in use
		if(mActiveSheet == NULL)
			return false;

		// If the mouse is disabled, we do not accept this injection of input
		if( !mMouseCursor->getEnabled() ) 
			return false;

		Point oldPos = mMouseCursor->getPosition();
		// Update cursor's position as seen on the screen.
		mMouseCursor->setPosition(xPixelPosition,yPixelPosition);

		// Determine the offset and inject a mouse movement input.
		return injectMouseMove(xPixelPosition - oldPos.x,yPixelPosition - oldPos.y);
	}

	bool GUIManager::injectMouseWheelChange(float delta)
	{
		// do nothing if no active sheet is in use
		if(mActiveSheet == NULL)
			return false;

		// If the mouse is disabled, we do not accept this injection of input
		if( !mMouseCursor->getEnabled() ) 
			return false;

		MouseEventArgs args(mWidgetUnderMouseCursor);
		args.position = mMouseCursor->getPosition();
		args.wheelChange = delta;
		args.buttonMask = mButtonMask;
		args.keyModifiers = mKeyModifiers;

		if(mGUIManagerDesc.scrollLastClicked)
		{
			if(mLastClickedWidget != NULL)
			{
				Widget* w = mLastClickedWidget->getScrollableContainerWidget();
				if(w != NULL)
				{
					w->fireWidgetEvent(WIDGET_EVENT_MOUSE_WHEEL,args);
					//injectMouseMove(0,0);
					return true;
				}
			}
		}
		else
		{
			if(mWidgetUnderMouseCursor != NULL)
			{
				Widget* w = mWidgetUnderMouseCursor->getScrollableContainerWidget();
				if(w != NULL)
				{
					w->fireWidgetEvent(WIDGET_EVENT_MOUSE_WHEEL,args);
					return (mWidgetUnderMouseCursor != NULL);
				}
			}
		}

		return (mWidgetUnderMouseCursor != NULL);
	}

	bool GUIManager::isSupportedCodePoint(Ogre::UTFString::unicode_char c)
	{		return (mGUIManagerDesc.supportedCodePoints.find(c) != mGUIManagerDesc.supportedCodePoints.end());
	}

	void GUIManager::hoverTimerCallback()
	{
		if(mWidgetUnderMouseCursor != NULL)
		{
			WidgetEventArgs args(mWidgetUnderMouseCursor);
			mWidgetUnderMouseCursor->fireWidgetEvent(WIDGET_EVENT_ON_HOVER, args);
		}
	}

	void GUIManager::notifyViewportDimensionsChanged()
	{
		if(mGUIManagerDesc.viewport == NULL)
		{
			mViewportWidth = 0;
			mViewportHeight = 0;
		}
		else
		{
			mViewportWidth = mGUIManagerDesc.viewport->getActualWidth();
			mViewportHeight = mGUIManagerDesc.viewport->getActualHeight();
		}

		if(mActiveSheet != NULL)
			mActiveSheet->notifyViewport(&mViewportWidth,&mViewportHeight);
	}

	KeyCode GUIManager::processNumPadKeyCode(const KeyCode& kc)
	{
		// If NumLock key is not down
		if(!(mKeyModifiers & NUMLOCK))
		{
			switch(kc)
			{
			case KC_NUMPAD0:	return KC_INSERT;
			case KC_NUMPAD1:	return KC_END;
			case KC_NUMPAD2:	return KC_DOWN;
			case KC_NUMPAD3:	return KC_PGDOWN;
			case KC_NUMPAD4:	return KC_LEFT;
			case KC_NUMPAD6:	return KC_RIGHT;
			case KC_NUMPAD7:	return KC_HOME;
			case KC_NUMPAD8:	return KC_UP;
			case KC_NUMPAD9:	return KC_PGUP;
			case KC_DECIMAL:	return KC_DELETE;
			default: return kc;
			}
		}

		return kc;
	}

	void GUIManager::renderQueueStarted(Ogre::uint8 id, const std::string& invocation, bool& skipThisQueue)
	{
	}

	void GUIManager::renderQueueEnded(Ogre::uint8 id, const std::string& invocation, bool& repeatThisQueue)
	{
		// Perform rendering of GUI
		if(mGUIManagerDesc.queueID == id)
		{
			draw();
		}
	}

	void GUIManager::setActiveSheet(Sheet* s)
	{
		if(mActiveSheet != NULL)
		{
			if(mWidgetUnderMouseCursor != NULL)
			{
				// Create MouseEventArgs, for use with any fired events
				MouseEventArgs args(mWidgetUnderMouseCursor);
				args.position = mMouseCursor->getPosition();
				args.buttonMask = mButtonMask;
				args.keyModifiers = mKeyModifiers;

				mWidgetUnderMouseCursor->fireWidgetEvent(WIDGET_EVENT_MOUSE_LEAVE,args);
			}
		}

		mActiveSheet = s;
		mWidgetUnderMouseCursor = mActiveSheet;
		mLastClickedWidget = NULL;

		if(mActiveSheet != NULL)
		{
			mActiveSheet->notifyViewport(&mViewportWidth,&mViewportHeight);

			mActiveSheet->_setGUIManager(this);

			// In the case where a sheet was unloaded, and later reloaded, make sure the Sheet is drawn correctly.
			// For example, prevent the scenario where a particular button was in the "Over" state when unloaded, but on
			// loading, should not be in the "Over" state because the cursor is not over the button.
			injectMouseMove(0,0);
		}
	}

	void GUIManager::setAutoRepeatInterval(float timeInSeconds)
	{
		mGUIManagerDesc.autoRepeatInterval = timeInSeconds;

		if(mAutoRepeatInjectionTimer != NULL)
		{
			mAutoRepeatInjectionTimer->reset();
			mAutoRepeatInjectionTimer->setTimePeriod(timeInSeconds);
		}
	}

	void GUIManager::setInjectNumPadKeyCodesAsUnicodeChars(bool inject)
	{
		mGUIManagerDesc.injectNumPadKeyCodesAsUnicodeChars = inject;
	}

	void GUIManager::setProcessNumPadKeyCodes(bool process)
	{
		mGUIManagerDesc.processNumPadKeyCodes = process;
	}

	void GUIManager::setRenderQueueID(Ogre::uint8 id)
	{
		mGUIManagerDesc.queueID = id;
	}

	void GUIManager::setSceneManager(Ogre::SceneManager* sm)
	{
		// remove listener from previous scene manager
		if(mGUIManagerDesc.sceneManager != NULL)
			mGUIManagerDesc.sceneManager->removeRenderQueueListener(this);
		
		// update
		mGUIManagerDesc.sceneManager = sm;

		// add listener to new scene manager
		if(mGUIManagerDesc.sceneManager != NULL)
			mGUIManagerDesc.sceneManager->addRenderQueueListener(this);
	}

	void GUIManager::setScrollLastClicked(bool scroll)
	{
		mGUIManagerDesc.scrollLastClicked = scroll;
	}

	void GUIManager::setSupportedCodePoints(const std::set<Ogre::UTFString::code_point>& list)
	{
		mGUIManagerDesc.supportedCodePoints = list;
	}

	void GUIManager::setTimeToAutoRepeat(float timeInSeconds)
	{
		mGUIManagerDesc.timeToAutoRepeat = timeInSeconds;

		if(mAutoRepeatEnableTimer != NULL)
		{
			mAutoRepeatEnableTimer->reset();
			mAutoRepeatEnableTimer->setTimePeriod(timeInSeconds);
		}
	}

	void GUIManager::setNumLock(bool on)
	{
		if(on)
			mKeyModifiers |= NUMLOCK;
		else
			mKeyModifiers &= ~NUMLOCK;
	}

	void GUIManager::setViewport(Ogre::Viewport* vp)
	{
		mGUIManagerDesc.viewport = vp;

		if(vp == NULL)
		{
			mViewportWidth = 0;
			mViewportHeight = 0;
		}
		else
		{
			mViewportWidth = vp->getActualWidth();
			mViewportHeight = vp->getActualHeight();
		}
	}

	void GUIManager::translateNumPadKeyCodesToUnicodeChars(const KeyCode& kc)
	{
		// If NumLock key is down
		if(mKeyModifiers | NUMLOCK)
		{
			Ogre::UTFString::unicode_char c = 0;
			switch(kc)
			{
			case KC_NUMPAD0:	c = 48;	break;
			case KC_NUMPAD1:	c = 49;	break;
			case KC_NUMPAD2:	c = 50;	break;
			case KC_NUMPAD3:	c = 51;	break;
			case KC_NUMPAD4:	c = 52;	break;
			case KC_NUMPAD5:	c = 53;	break;
			case KC_NUMPAD6:	c = 54;	break;
			case KC_NUMPAD7:	c = 55;	break;
			case KC_NUMPAD8:	c = 56;	break;
			case KC_NUMPAD9:	c = 57;	break;
			case KC_DECIMAL:	c = 46;	break;
			case KC_DIVIDE:		c = 47; break;
			default: break;
			}

			if(c != 0)
				injectChar(c);
		}
	}
}
