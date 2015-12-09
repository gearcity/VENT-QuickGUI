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

#ifndef QUICKGUIIMAGE_H
#define QUICKGUIIMAGE_H

#include "QuickGUIWidget.h"
#include "QuickGUITimerManager.h"

namespace QuickGUI
{
	// forward declarations
	class Brush;

	class _QuickGUIExport ImageDesc :
			public WidgetDesc
	{
	public:
		friend class DescFactory;
	protected:
		ImageDesc();
		virtual ~ImageDesc() {}
	public:

		/// Allows drawing of background after center image,
		/// which can achieve fancy effects, ie parts of frame drawn over image
		bool			image_drawBackgroundLast;
		/// Name of the image file to display
		Ogre::String	image_imageName;
		/// Number of degrees to rotate the image
		int				image_rotationInDegrees;
		bool			image_tileImage;
		/// Useful for applying render to texture (RTT) to Image
		bool			image_updateEveryFrame;

		/**
		* Returns the class of Desc object this is.
		*/
		virtual Ogre::String getClass() { return "ImageDesc"; }
		/**
		* Returns the class of Widget this desc object is meant for.
		*/
		virtual Ogre::String getWidgetClass() { return "Image"; }

		/**
		* Restore properties to default values
		*/
		virtual void resetToDefault();

		/**
		* Outlines how the desc class is written to XML and read from XML.
		*/
		virtual void serialize(SerialBase* b);
	};

	class _QuickGUIExport Image :
		public Widget
	{
	public:
		// Skin Constants
		static const Ogre::String BACKGROUND;
		// Define Skin Structure
		static void registerSkinDefinition();
	public:
		friend class WidgetFactory;
	public:

		/**
		* Internal function, do not use.
		*/
		virtual void _initialize(WidgetDesc* d);

		/**
		* Returns the class name of this Widget.
		*/
		virtual Ogre::String getClass();
		/**
		* Return true if the image background is drawn after the image, false otherwise.
		*/
		bool getDrawBackgroundLast();
		/**
		* Returns the name of the image file used for display.
		*/
		Ogre::String getImageName();
		/**
		* Gets the current rotation of the Image.
		*/
		int getRotation();
		/**
		* Returns true if image is tiled within client dimensions, false otherwise.
		*/
		bool getTileImage();
		/**
		* Gets whether or not the Image will be redrawn every frame.
		*/
		bool getUpdateEveryFrame();

		/**
		* Numbers of degrees to rotate this image in addition to its current rotation, if any.
		* NOTE: Rotation occurs counter clockwise.
		*/
		void rotate(int numDegrees);

		/**
		* Sets whether the image background should be drawn after or before the image.
		* This allows for effects where the background covers part of the image.
		*/
		void setDrawBackgroundLast(bool last);
		/**
		* Sets the name of the image file to be displayed.  For example, name could could be
		* qgui.image.png.
		* NOTE: Image must be listed within an Ogre resource directory.
		* NOTE: Setting the name to "" will result in only the background showing.
		*/
		void setImage(const Ogre::String& name);
		/**
		* Sets the number of degrees to rotate this image, 0 means no rotation occurs.
		* NOTE: Rotation occurs counter clockwise.
		*/
		void setRotation(int numDegrees);
		/**
		* Sets whether or not Image will tile within client dimensions.
		*/
		void setTileImage(bool tile);
		/**
		* Sets whether or not the Image will be redrawn every frame.
		* NOTE: This also causes the Images parent window to be re-drawn, which can be costly.
		* The main use for this is for RTT. (Render to Texture)
		*/
		void setUpdateEveryFrame(bool update);

	public:
		// Here we have to call out any protected Widget set accesors we want to expose

		using Widget::drag;
		using Widget::resize;
		using Widget::setConsumeKeyboardEvents;
		using Widget::setDimensions;
		using Widget::setDragable;
		using Widget::setHeight;
		using Widget::setHorizontalAnchor;
		using Widget::setMaxSize;
		using Widget::setMinSize;
		using Widget::setPosition;
		using Widget::setPositionRelativeToParentClientDimensions;
		using Widget::setQueryFlags;
		using Widget::setResizeFromAllSides;
		using Widget::setResizeFromBottom;
		using Widget::setResizeFromLeft;
		using Widget::setResizeFromRight;
		using Widget::setResizeFromTop;
		using Widget::setScrollable;
		using Widget::setSerialize;
		using Widget::setSize;
		using Widget::setTransparencyPicking;
		using Widget::setVerticalAnchor;
		using Widget::setVisible;
		using Widget::setWidth;

	protected:
		Image(const Ogre::String& name);
		virtual ~Image();

		ImageDesc* mDesc;

		/**
		* Outlines how the widget is drawn to the current render target
		*/
		virtual void onDraw();

		/// Timer that toggles cursor on and off.
		Timer* mUpdateTimer;
		/**
		* Forces an update of the Image.
		*/
		void updateTimerCallback();

		/**
		* Sets whether the widget will receive mouse over events simply by having the mouse over
		* its texture dimensions, or only when the cursor is over non transparent parts.
		*/
		void setTransparencyPicking(bool transparencyPicking);

		// small helper functions for code re-usability
		void _drawImageBackground(Brush* brush);
		void _drawImage(Brush* brush);

	private:
	};
}

#endif
