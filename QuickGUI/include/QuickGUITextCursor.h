#ifndef QUICKGUITEXTCURSOR_H
#define QUICKGUITEXTCURSOR_H

#include "QuickGUIBrush.h"
#include "QuickGUIDesc.h"
#include "QuickGUIExportDLL.h"
#include "QuickGUISkinTypeManager.h"

#include "OgreFrameListener.h"

namespace QuickGUI
{
	// forward declarations
	class Widget;

	class _QuickGUIExport TextCursor
	{
	public:
		// Skin Constants
		static const Ogre::String BACKGROUND;
		// Define Skin Structure
		static void registerSkinDefinition();
	public:
		TextCursor(Widget* owner);
		~TextCursor();

		/**
		* Returns the position of the TextCursor.
		* NOTE: The position is relative to the viewport the TextCursor will be drawn in. (global position)
		*/
		Point getPosition();
		/**
		* Returns the size of the TextCursor.
		*/
		Size getSize();
		/**
		* Returns true if the cursor is visible, false otherwise.
		*/
		bool getVisible();

		/**
		* Outlines how the widget is drawn to the current render target
		*/
		void onDraw();

		/**
		* Scales the TextCursor to match the height given.
		*/
		void setHeight(float heightInPixels);
		/**
		* Sets the position of the TextCursor.
		* NOTE: The position is relative to the viewport the TextCursor will be drawn in. (global position)
		* NOTE: The position of the TextCursor is offset horizontally to 
		* center the cursor around the x coordinate.
		*/
		void setPosition(Point& p);
		/**
		* Sets the "type" of this Text Cursor.  For example you
		* can create several types of Text Cursors: "default", "italic", "plain", etc.
		* NOTE: The type property determines what is drawn to the screen.
		*/
		void setSkinType(const Ogre::String type);
		/**
		* Sets the cursor visible or not.
		*/
		void setVisible(bool visible);

	protected:
		Rect mDimensions;
		bool mVisible;

		Widget* mOwner;

		SkinType* mSkinType;
		// storing pointer to save lookup time for drawing operation.
		SkinElement* mSkinElement;

	private:
	};
}

#endif
