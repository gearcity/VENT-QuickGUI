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

#ifndef QUICKGUILISTIMAGEITEM_H
#define QUICKGUILISTIMAGEITEM_H

#include "QuickGUIListItem.h"

namespace QuickGUI
{
	class _QuickGUIExport ListImageItemDesc :
		public ListItemDesc
	{
	public:
		friend class DescFactory;
	protected:
		ListImageItemDesc();
		virtual ~ListImageItemDesc() {}
	public:

		Ogre::String listimageitem_imageName;
		bool listimageitem_tileImage;

		/**
		* Returns the class of Desc object this is.
		*/
		virtual Ogre::String getClass() { return "ListImageItemDesc"; }
		/**
		* Returns the class of Widget this desc object is meant for.
		*/
		virtual Ogre::String getWidgetClass() { return "ListImageItem"; }

		/**
		* Restore properties to default values
		*/
		virtual void resetToDefault();

		/**
		* Outlines how the desc class is written to XML and read from XML.
		*/
		virtual void serialize(SerialBase* b);
	};

	class _QuickGUIExport ListImageItem :
		public ListItem
	{
	public:
		// Define Skin Structure
		static void registerSkinDefinition();
	public:
		// ComboBox works with ListTextItems specifically and needs to be able to destroy them.
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
		* Sets whether or not Image will tile within client dimensions.
		*/
		void setTileImage(bool tile);

	protected:
		ListImageItem(const Ogre::String& name);
		virtual ~ListImageItem();

		ListImageItemDesc* mDesc;

		/**
		* Outlines how the widget is drawn to the current render target
		*/
		virtual void onDraw();

	private:
	};
}

#endif
