#include "QuickGUIMouseCursor.h"
#include "QuickGUIManager.h"
#include "QuickGUISkinDefinitionManager.h"
#include "QuickGUISkinDefinition.h"
#include "QuickGUISkinType.h"
#include "QuickGUISkinTypeManager.h"
#include "QuickGUIBrush.h"
#include "QuickGUIEventHandlerManager.h"

#include "OgreImage.h"

namespace QuickGUI
{
	const Ogre::String MouseCursor::TEXTURE = "texture";

	MouseCursorDesc::MouseCursorDesc()
	{
		clipOnEdges = false;
		enabled = true;
		opacity = 1.0;
		skin = "qgui";
		visible = true;
		guiManager = NULL;
		brushFilterMode = BRUSHFILTER_NONE;

		for(int index = 0; index < MOUSE_CURSOR_EVENT_COUNT; ++index)
			userHandlers[index] = "";
	}

	void MouseCursor::registerSkinDefinition()
	{
		SkinDefinition* d = OGRE_NEW_T(SkinDefinition,Ogre::MEMCATEGORY_GENERAL)("MouseCursor");
		d->defineSkinElement(TEXTURE);
		d->definitionComplete();

		SkinDefinitionManager::getSingleton().registerSkinDefinition("MouseCursor",d);
	}

	MouseCursor::MouseCursor(const MouseCursorDesc& d) :
		mSkinType("default"),
		mDefaultSkinType("default")
	{
		mMouseCursorDesc.guiManager = d.guiManager;
		mSkinTypeManager = SkinTypeManager::getSingletonPtr();

		for(int i = 0; i < 4; ++i)
			mEnteredBorders[i] = false;

		// Update Size of Cursor to match Skin Texture
		Ogre::Image i;
		i.load(mSkinTypeManager->getSkinType("MouseCursor",mSkinType)->getSkinElement(TEXTURE)->getTextureName(),Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
		// update cursor size to match texture used
		setSize(i.getWidth(),i.getHeight());

		// Set default position
		setPosition(0,0);
	}

	MouseCursor::~MouseCursor()
	{
		// Clean up all user defined event handlers.
		for(int index = 0; index < MOUSE_CURSOR_EVENT_COUNT; ++index)
		{
			for(std::vector<EventHandlerSlot*>::iterator it = mEventHandlers[index].begin(); it != mEventHandlers[index].end(); ++it)
				OGRE_DELETE_T((*it),EventHandlerSlot,Ogre::MEMCATEGORY_GENERAL);
		}
	}

	void MouseCursor::_setSkinType(const Ogre::String type)
	{
		if(mSkinType == type)
			return;

		mSkinType = type;
		
		Ogre::Image i;
		i.load(mSkinTypeManager->getSkinType("MouseCursor",mSkinType)->getSkinElement(TEXTURE)->getTextureName(),Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
		// update cursor size to match texture used
		setSize(i.getWidth(),i.getHeight());
	}

	void MouseCursor::addUserDefinedMouseCursorEventHandler(MouseCursorEvent EVENT, const Ogre::String& handlerName)
	{
		mMouseCursorDesc.userHandlers[EVENT] = handlerName;
	}

	void MouseCursor::addCursorEventHandler(MouseCursorEvent EVENT, EventHandlerSlot* function)
	{
		mEventHandlers[EVENT].push_back(function);
	}

	void MouseCursor::clearUserDefinedMouseCursorEventHandler(MouseCursorEvent EVENT)
	{
		mMouseCursorDesc.userHandlers[EVENT] = "";
	}

	void MouseCursor::draw()
	{
		// If cursor is not visible, return
		if(!mMouseCursorDesc.visible)
			return;
		
		// If cursor is on a border and clipOnEdges is true, return
		if(mMouseCursorDesc.clipOnEdges)
		{
			for(int i = 0; i < 4; ++i)
			{
				if(mEnteredBorders[i])
				{
					return;
				}
			}
		}

		Brush* b = Brush::getSingletonPtr();
		b->setColor(ColourValue(1,1,1,mMouseCursorDesc.opacity));
		b->setRenderTarget(NULL);
		b->setFilterMode(mMouseCursorDesc.brushFilterMode);
		b->drawSkinElement(mDimensions,mSkinTypeManager->getSkinType("MouseCursor",mSkinType)->getSkinElement(TEXTURE));
	}

	BrushFilterMode MouseCursor::getBrushFilterMode()
	{
		return mMouseCursorDesc.brushFilterMode;
	}

	bool MouseCursor::getClipOnEdges()
	{
		return mMouseCursorDesc.clipOnEdges;
	}

	bool MouseCursor::getEnabled()
	{
		return true;
	}

	float MouseCursor::getOpacity()
	{
		return mMouseCursorDesc.opacity;
	}

	Point MouseCursor::getPosition()
	{
		return Point(mDimensions.position.x + (mDimensions.size.width/2.0),mDimensions.position.y + (mDimensions.size.height/2.0));
	}

	Ogre::String MouseCursor::getSkinTypeName()
	{
		return mDefaultSkinType;
	}

	bool MouseCursor::getVisible()
	{
		return mMouseCursorDesc.visible;
	}

	void MouseCursor::setBrushFilterMode(BrushFilterMode m)
	{
		mMouseCursorDesc.brushFilterMode = m;
	}

	void MouseCursor::setClipOnEdges(bool clip)
	{
		mMouseCursorDesc.clipOnEdges = clip;
	}

	void MouseCursor::setEnabled(bool enable)
	{
		if(enable == mMouseCursorDesc.enabled)
			return;

		mMouseCursorDesc.enabled = enable;

		// Fire enabled changed event.
		MouseEventArgs args(NULL);
		args.position = getPosition();

		for(std::vector<EventHandlerSlot*>::iterator it = mEventHandlers[MOUSE_CUSSOR_EVENT_ENABLED_CHANGED].begin(); it != mEventHandlers[MOUSE_CUSSOR_EVENT_ENABLED_CHANGED].end(); ++it)
			(*it)->execute(args);

		if(mMouseCursorDesc.userHandlers[MOUSE_CUSSOR_EVENT_ENABLED_CHANGED] != "")
			EventHandlerManager::getSingletonPtr()->executEventHandler(mMouseCursorDesc.userHandlers[MOUSE_CUSSOR_EVENT_ENABLED_CHANGED],args);
	}

	void MouseCursor::setOpacity(float opacity)
	{
		if(opacity < 0.0)
			opacity = 0.0;
		else if (opacity > 1.0)
			opacity = 1.0;

		mMouseCursorDesc.opacity = opacity;
	}

	void MouseCursor::setPosition(float xPosition, float yPosition)
	{
		mDimensions.position.x = xPosition - (mDimensions.size.width/2.0);
		mDimensions.position.y = yPosition - (mDimensions.size.height/2.0);

		bool fireBorderEnterEvent = false;
		bool fireBorderLeaveEvent = false;

		// If cursor horizontal position is on left edge of viewport
		if(xPosition <= 0)
		{
			// If we haven't entered the border already, fire event
			if(!mEnteredBorders[0])
			{
				mEnteredBorders[0] = true;
				fireBorderEnterEvent = true;
			}
		}
		// Else if cursor horizontal position is on right edge of viewport
		else if(xPosition >= (mMouseCursorDesc.guiManager->getViewport()->getActualWidth() - 1))
		{
			// If we haven't entered the border already, fire event
			if(!mEnteredBorders[2])
			{
				mEnteredBorders[2] = true;
				fireBorderEnterEvent = true;
			}
		}
		// Else cursor horizontal position lies in between left and right edge of viewport
		else
		{
			if(mEnteredBorders[0] || mEnteredBorders[2])
			{
				mEnteredBorders[0] = false;
				mEnteredBorders[2] = false;
				fireBorderLeaveEvent = true;
			}
		}

		// If cursor vertical position is on top edge of viewport
		if(yPosition <= 0)
		{
			// If we haven't entered the border already, fire event
			if(!mEnteredBorders[1])
			{
				mEnteredBorders[1] = true;
				fireBorderEnterEvent = true;
			}
		}
		// Else if cursor vertical position is on bottom edge of viewport
		else if(yPosition >= (mMouseCursorDesc.guiManager->getViewport()->getActualHeight() - 1))
		{
			// If we haven't entered the border already, fire event
			if(!mEnteredBorders[3])
			{
				mEnteredBorders[3] = true;
				fireBorderEnterEvent = true;
			}
		}
		// Else cursor vertical position lies in between top and bottom edge of viewport
		else
		{
			if(mEnteredBorders[1] || mEnteredBorders[3])
			{
				mEnteredBorders[1] = false;
				mEnteredBorders[3] = false;
				fireBorderLeaveEvent = true;
			}
		}

		// Fire events
		if(fireBorderEnterEvent)
		{
			MouseEventArgs args(NULL);
			args.position = getPosition();

			for(std::vector<EventHandlerSlot*>::iterator it = mEventHandlers[MOUSE_CURSOR_EVENT_BORDER_ENTER].begin(); it != mEventHandlers[MOUSE_CURSOR_EVENT_BORDER_ENTER].end(); ++it)
				(*it)->execute(args);

			if(mMouseCursorDesc.userHandlers[MOUSE_CURSOR_EVENT_BORDER_ENTER] != "")
				EventHandlerManager::getSingletonPtr()->executEventHandler(mMouseCursorDesc.userHandlers[MOUSE_CURSOR_EVENT_BORDER_ENTER],args);
		}

		if(fireBorderLeaveEvent)
		{
			MouseEventArgs args(NULL);
			args.position = getPosition();

			for(std::vector<EventHandlerSlot*>::iterator it = mEventHandlers[MOUSE_CURSOR_EVENT_BORDER_LEAVE].begin(); it != mEventHandlers[MOUSE_CURSOR_EVENT_BORDER_LEAVE].end(); ++it)
				(*it)->execute(args);

			if(mMouseCursorDesc.userHandlers[MOUSE_CURSOR_EVENT_BORDER_LEAVE] != "")
				EventHandlerManager::getSingletonPtr()->executEventHandler(mMouseCursorDesc.userHandlers[MOUSE_CURSOR_EVENT_BORDER_LEAVE],args);
		}
	}

	void MouseCursor::setPosition(const Point& p)
	{
		setPosition(p.x,p.y);
	}

	void MouseCursor::setSize(float pixelWidth, float pixelHeight)
	{
		Point currentPosition = getPosition();

		mDimensions.size.width = pixelWidth;
		mDimensions.size.height = pixelHeight;

		mDimensions.position.x = currentPosition.x - (mDimensions.size.width/2.0);
		mDimensions.position.y = currentPosition.y - (mDimensions.size.height/2.0);
	}

	void MouseCursor::setSkinType(const Ogre::String type)
	{
		mSkinType = type;
		mDefaultSkinType = type;

		Ogre::Image i;
		i.load(mSkinTypeManager->getSkinType("MouseCursor",mSkinType)->getSkinElement(TEXTURE)->getTextureName(),Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
		// update cursor size to match texture used
		setSize(i.getWidth(),i.getHeight());
	}

	void MouseCursor::setVisible(bool visible)
	{
		mMouseCursorDesc.visible = visible;
	}
}
