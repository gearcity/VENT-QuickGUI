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

#ifndef QUICKGUIPROPERTYGRIDTEXTPROPERTY_H
#define QUICKGUIPROPERTYGRIDTEXTPROPERTY_H

#include "QuickGUIPropertyGridItem.h"

namespace QuickGUI
{
	// forward declarations
	class TextBox;

	class _QuickGUIExport PropertyGridTextPropertyDesc :
		public PropertyGridItemDesc
	{
	public:
		friend class DescFactory;
	protected:
		PropertyGridTextPropertyDesc();
		virtual ~PropertyGridTextPropertyDesc() {}
	public:

		/**
		* Returns the class of Desc object this is.
		*/
		virtual Ogre::String getClass() { return "PropertyGridTextPropertyDesc"; }
		/**
		* Returns the class of Widget this desc object is meant for.
		*/
		virtual Ogre::String getWidgetClass() { return "PropertyGridTextProperty"; }

		/**
		* Restore properties to default values
		*/
		virtual void resetToDefault();

		/**
		* Outlines how the desc class is written to XML and read from XML.
		*/
		virtual void serialize(SerialBase* b);
	};

	class _QuickGUIExport PropertyGridTextProperty :
		public PropertyGridItem
	{
	public:
		// Define Skin Structure
		static void registerSkinDefinition();
	public:
		// TreeView has to set positions, toggle visibility to manage ListItems
		friend class TreeView;
		// Section updates Property TreeView pointer
		friend class PropertyGridSection;

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

	protected:
		PropertyGridTextProperty(const Ogre::String& name);
		virtual ~PropertyGridTextProperty();

		PropertyGridTextPropertyDesc* mDesc;

		TextBox* mTextBox;

		virtual void updateFieldSkin();

	private:
	};
}

#endif
