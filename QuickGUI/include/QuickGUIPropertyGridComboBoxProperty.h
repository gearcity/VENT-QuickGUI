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

#ifndef QUICKGUIPROPERTYGRIDCOMBOBOXPROPERTY_H
#define QUICKGUIPROPERTYGRIDCOMBOBOXPROPERTY_H

#include "QuickGUIPropertyGridItem.h"

namespace QuickGUI
{
	// forward declarations
	class ComboBox;
	class ListItem;
	class ListImageItem;
	class ListTextItem;

	class _QuickGUIExport PropertyGridComboBoxPropertyDesc :
		public PropertyGridItemDesc
	{
	public:
		friend class DescFactory;
	protected:
		PropertyGridComboBoxPropertyDesc();
		virtual ~PropertyGridComboBoxPropertyDesc() {}
	public:

		/**
		* Returns the class of Desc object this is.
		*/
		virtual Ogre::String getClass() { return "PropertyGridComboBoxPropertyDesc"; }
		/**
		* Returns the class of Widget this desc object is meant for.
		*/
		virtual Ogre::String getWidgetClass() { return "PropertyGridComboBoxProperty"; }

		/**
		* Restore properties to default values
		*/
		virtual void resetToDefault();

		/**
		* Outlines how the desc class is written to XML and read from XML.
		*/
		virtual void serialize(SerialBase* b);
	};

	class _QuickGUIExport PropertyGridComboBoxProperty :
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
		* Adds a ListImageItem to the end of the DropDownList.
		*/
		ListImageItem* createImageItem(const Ogre::String& imageName, int index = -1);
		ListImageItem* createImageItem(int index = -1);
		/**
		* Adds a ListTextItem to the end of the DropDownList
		*/
		ListTextItem* createTextItem(const Ogre::UTFString& text, int index = -1);
		ListTextItem* createTextItem(std::vector<TextSegment> segments, int index = -1);
		ListTextItem* createTextItem(int index = -1);

		/**
		* Removes an Item from the List.
		*/
		void destroyItem(unsigned int index);
		/**
		* Removes an Item from the List.
		*/
		void destroyItem(ListItem* i);

		/**
		* Returns the class name of this Widget.
		*/
		virtual Ogre::String getClass();
		/**
		* Gets the width of the Drop down MenuPanel.
		*/
		float getDropDownWidth();
		/**
		* Returns the Item at the index given.  If the index is invalid,
		* NULL is returned.
		*/
		ListItem* getItem(unsigned int index);
		/**
		* Gets the maximum height of the Drop Down List.
		*/
		float getMaxDropDownHeight();
		/**
		* Gets the number of ListItems within this List.
		*/
		int getNumberOfItems();
		/**
		* Returns a pointer to a list of selected items.
		*/
		ListItem* getSelectedItem();

		/**
		* Hides Drop Down List.
		*/
		void hideDropDownList();

		/**
		* Selects the item at index given.  If index is not valid,
		* all items will be de-selected.
		*/
		void selectItem(unsigned int index);
		/**
		* Sets the width of the Drop down MenuPanel.
		*/
		void setDropDownWidth(float width);
		/**
		* Sets the maximum height of the Drop Down List.
		*/
		void setMaxDropDownHeight(float height);
		/**
		* Shows the Drop Down List.
		*/
		void showDropDownList();

	protected:
		PropertyGridComboBoxProperty(const Ogre::String& name);
		virtual ~PropertyGridComboBoxProperty();

		PropertyGridComboBoxPropertyDesc* mDesc;

		ComboBox* mComboBox;

		void updateFieldSkin();

	private:
	};
}

#endif
