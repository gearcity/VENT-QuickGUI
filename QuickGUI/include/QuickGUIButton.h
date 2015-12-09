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

#ifndef QUICKGUIBUTTON_H
#define QUICKGUIBUTTON_H

#include "QuickGUILabel.h"

namespace Ogre
{
	// forward declarations
	class Font;
}

namespace QuickGUI
{
	class _QuickGUIExport ButtonDesc :
			public LabelDesc
	{
	public:
		friend class DescFactory;
	protected:
		ButtonDesc();
		virtual ~ButtonDesc() {}
	public:

		/// Name of the image file to display
		Ogre::String	button_imageName;
		bool			button_tileImage;

		/**
		* Returns the class of Desc object this is.
		*/
		virtual Ogre::String getClass() { return "ButtonDesc"; }
		/**
		* Returns the class of Widget this desc object is meant for.
		*/
		virtual Ogre::String getWidgetClass() { return "Button"; }

		/**
		* Restore properties to default values
		*/
		virtual void resetToDefault();

		/**
		* Outlines how the desc class is written to XML and read from XML.
		*/
		virtual void serialize(SerialBase* b);
	};

	class _QuickGUIExport Button :
		public Label
	{
	public:
		// Skin Constants
		static const Ogre::String DEFAULT;
		static const Ogre::String OVER;
		static const Ogre::String DOWN;
		// Define Skin Structure
		static void registerSkinDefinition();
	public:		
		// ComboBox changes its buttons state.
		friend class ComboBox;

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
		* Returns the name of the image file used for display.
		*/
		Ogre::String getImageName();
		/**
		* Returns true if image is tiled within client dimensions, false otherwise.
		*/
		bool getTileImage();

		/**
		* Sets the name of the image file to be displayed.  For example, name could could be
		* qgui.image.png.
		* NOTE: Image must be listed within an Ogre resource directory.
		* NOTE: Setting the name to "" will result in only the background showing.
		*/
		void setImage(const Ogre::String& name);
		/**
		* Sets the text for this object.
		*/
		void setText(Ogre::UTFString s, Ogre::Font* fp, const ColourValue& cv);
		/**
		* Sets the text for this object.
		*/
		void setText(Ogre::UTFString s, const Ogre::String& fontName, const ColourValue& cv);
		/**
		* Sets the text for this object.
		*/
		void setText(Ogre::UTFString s, const ColourValue& cv);
		/**
		* Sets the text for this object.
		*/
		void setText(Ogre::UTFString s);
		/**
		* Sets the Text using Text Segments.
		*/
		void setText(std::vector<TextSegment> segments);
		/**
		* Sets whether or not Image will tile within client dimensions.
		*/
		void setTileImage(bool tile);

		/**
		* Recalculate Client dimensions, relative to Widget's actual dimensions.
		*/
		virtual void updateClientDimensions();

	public:
		// Here we have to call out any protected Widget set accesors we want to expose

		using Widget::setMoveWindowOnDrag;

	protected:
		Button(const Ogre::String& name);
		virtual ~Button();

		ButtonDesc* mDesc;

		/**
		* Outlines how the widget is drawn to the current render target
		*/
		virtual void onDraw();

		/**
		* Handler defined to implement the button's default natural behavior. (BUTTON_STATE_OVER)
		*/
		void onMouseEnter(const EventArgs& args);
		/**
		* Handler defined to implement the button's default natural behavior. (BUTTON_STATE_DEFAULT)
		*/
		void onMouseLeave(const EventArgs& args);
		/**
		* Handler defined to implement the button's default natural behavior. (BUTTON_STATE_DOWN)
		*/
		void onMouseLeftButtonDown(const EventArgs& args);
		/**
		* Handler defined to implement the button's default natural behavior. (BUTTON_STATE_OVER)
		*/
		void onMouseLeftButtonUp(const EventArgs& args);

		// Updates the SkinElement used for border detection and client area calculations.
		virtual void updateSkinElement();

	private:
	};
}

#endif
