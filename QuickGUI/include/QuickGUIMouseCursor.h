#ifndef QUICKGUIMOUSECURSOR_H
#define QUICKGUIMOUSECURSOR_H

#include "QuickGUIBrushEnums.h"
#include "QuickGUIEventArgs.h"
#include "QuickGUIEventTypes.h"
#include "QuickGUIExportDLL.h"
#include "QuickGUIEventHandlerPointer.h"
#include "QuickGUIRect.h"
#include "QuickGUIOgreEquivalents.h"

#include "OgrePrerequisites.h"

namespace QuickGUI
{
	// forward declaration
	class GUIManager;
	class SkinTypeManager;

	class _QuickGUIExport MouseCursorDesc
	{
	public:
		MouseCursorDesc();

		BrushFilterMode	brushFilterMode;
		/// whether or not to hide the mouse when it touches viewport edges
		bool clipOnEdges;
		bool enabled;
		float opacity;
		Ogre::String skin;
		// This will be set by the GUIManager creating the MouseCursor.
		GUIManager* guiManager;
		Ogre::String userHandlers[MOUSE_CURSOR_EVENT_COUNT];
		bool visible;
	};

	class _QuickGUIExport MouseCursor
	{
	public:
		friend class GUIManager;
	public:
		// Skin Constants
		static const Ogre::String TEXTURE;
		// Define Skin Structure
		static void registerSkinDefinition();
	public:

		/**
		* Adds a user-defined event handler to this widget.  The difference between this function and the addWidgetEventHandler
		* is that the handlerName gets serialized with the Widget. This means you can defined handlers outside the creation of the
		* GUI Layout, and when the sheet is loaded from disk, all of the handlers will work properly.
		* NOTE: Only 1 user defined Event Handler can be assigned to an Event. Old values will be overwritten.
		* NOTE: A handler of "" essentially removes the handler.
		* NOTE: If no eventHandler exists with the name provided, nothing will happen.
		*/
		void addUserDefinedMouseCursorEventHandler(MouseCursorEvent EVENT, const Ogre::String& handlerName);
		/** Adds an event handler
			@param
				EVENT Defined events, for example: MOUSE_CUSSOR_EVENT_ENABLED_CHANGED, MOUSE_CURSOR_EVENT_BORDER_LEAVE, etc.
            @param
                function member function assigned to handle the event.  Given in the form of myClass::myFunction.
				Function must return bool, and take QuickGUI::EventArgs as its parameters.
            @param
                obj particular class instance that defines the handler for this event.  Here is an example:
				addWidgetEventHandler(QuickGUI::MOUSE_CUSSOR_EVENT_ENABLED_CHANGED,myClass::myFunction,this);
			@note
				Multiple user defined event handlers can be defined for an event.  All added event handlers will be called
				whenever the event is fired.
			@note
				You may see Visual Studio give an error warning such as "error C2660: 'QuickGUI::Widget::addWidgetEventHandler' : function does not take 3 arguments".
				This is an incorrect error message.  Make sure your function pointer points to a function which returns void, and takes parameter "const EventArgs&".
        */
		template<typename T> void addCursorEventHandler(MouseCursorEvent EVENT, void (T::*function)(const EventArgs&), T* obj)
		{
			addCursorEventHandler(EVENT, OGRE_NEW_T(EventHandlerPointer<T>,Ogre::MEMCATEGORY_GENERAL)(function,obj));
		}
		void addCursorEventHandler(MouseCursorEvent EVENT, EventHandlerSlot* function);

		/**
		* Clears the user defined handler associated with the EVENT given. (sets the handlerName to "")
		*/
		void clearUserDefinedMouseCursorEventHandler(MouseCursorEvent EVENT);

		void draw();

		/**
		* Gets filtering mode used for drawing.
		*/
		BrushFilterMode getBrushFilterMode();
		/**
		* Returns true if the cursor will be hidden when touching the viewport edges, false otherwise.
		*/
		bool getClipOnEdges();
		/**
		* Returns true if the cursor is able to inject mouse events into the GUIManager,
		* false otherwise.
		*/
		bool getEnabled();
		/**
		* Returns the amount of transparency of the cursor. 0.0 means fully transparent, 1.0 means fully opaque.
		*/
		float getOpacity();
		/**
		* Returns the pixel location of the cursor as it appears on the screen.
		*/
		Point getPosition();
		/**
		* Returns the name of the skin type used by the cursor.
		*/
		Ogre::String getSkinTypeName();
		/**
		* Returns true if the cursor is drawn on the screen, false otherwise.
		*/
		bool getVisible();

		/**
		* Sets the filtering mode to use (See BrushFilterMode)
		*/
		void setBrushFilterMode(BrushFilterMode m);
		/**
		* Sets whether the cursor will be hidden or shown when touching the viewport edges.
		*/
		void setClipOnEdges(bool clip);
		/**
		* Sets whether the cursor is able to inject mouse events into the GUIManager.
		*/
		void setEnabled(bool enable);
		/**
		* Sets the amount of transparency of the cursor. 0.0 means fully transparent, 1.0 means fully opaque.
		*/
		void setOpacity(float opacity);
		/**
		* Sets the "type" of mouse cursor.  For example you
		* can change between several types: "default", "hresize", "diag1resize", etc.
		* NOTE: The type property determines what is drawn to the screen.
		*/
		void setSkinType(const Ogre::String type);
		/**
		* Sets whether the cursor will be drawn on the screen.
		*/
		void setVisible(bool visible);

	protected:
		MouseCursor(const MouseCursorDesc& d);
		~MouseCursor();

		MouseCursorDesc mMouseCursorDesc;

		Rect mDimensions;

		// Current Skin type of the cursor (for example changing the cursor directly, ie resizing)
		Ogre::String mSkinType;
		// Last Skin type as set by a user (for example restoring the cursor)
		Ogre::String mDefaultSkinType;

		SkinTypeManager* mSkinTypeManager;

		/**
		* Internal function used to set the size of the cursor, as it is drawn on the screen.
		*/
		void setSize(float pixelWidth, float pixelHeight);

		// Left/Top/Right/Bottom
		bool mEnteredBorders[4];
		// Event handlers! One List per event
		std::vector<EventHandlerSlot*> mEventHandlers[MOUSE_CURSOR_EVENT_COUNT];

		/**
		* Sets the "type" of mouse cursor.  For example you
		* can change between several types: "default", "hresize", "diag1resize", etc.
		* NOTE: The type property determines what is drawn to the screen.
		* NOTE: This does not update the default skin type of the cursor, and is for internal use only.
		*/
		void _setSkinType(const Ogre::String type);

		/**
		* Updates the cursor so that it is drawn at the position given.
		*/
		void setPosition(float xPosition, float yPosition);
		/**
		* Updates the cursor so that it is drawn at the position given.
		*/
		void setPosition(const Point& p);

	private:
	};
}

#endif
