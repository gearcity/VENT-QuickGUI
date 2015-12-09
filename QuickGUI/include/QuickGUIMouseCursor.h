/*
-----------------------------------------------------------------------------
This source file is part of QuickGUI
For the latest info, see http://www.ogre3d.org/addonforums/viewforum.php?f=13

Copyright (c) 2009 Stormsong Entertainment

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

(http://opensource.org/licenses/mit-license.php)
-----------------------------------------------------------------------------
*/

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
		unsigned int queryFilter;
		Ogre::String skin;
		// This will be set by the GUIManager creating the MouseCursor.
		GUIManager* guiManager;
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
		* Returns the query filter of the mouse.  The query filter is used in combination with widget queryFlags to
		* determine widget detection.  If the filter and flags are ANDed together and result in a non-zero value,
		* the widget is detected.
		*/
		unsigned int getQueryFilter();
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
		* Sets the query filter of the mouse.  The query filter is used in combination with widget queryFlags to
		* determine widget detection.  If the filter and flags are ANDed together and result in a non-zero value,
		* the widget is detected.
		*/
		void setQueryFilter(unsigned int filter);
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
