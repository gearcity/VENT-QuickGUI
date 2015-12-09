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

#ifndef QUICKGUICONTEXTMENU_H
#define QUICKGUICONTEXTMENU_H

#include "QuickGUIException.h"
#include "QuickGUIWindow.h"

#include <vector>

namespace QuickGUI
{
	// forward declarations
	class Menu;
	class MenuItem;
	class MenuImageItem;
	class MenuTextItem;

	class MenuItemDesc;

	class _QuickGUIExport ContextMenuDesc :
			public WindowDesc
	{
	public:
		friend class DescFactory;
	protected:
		ContextMenuDesc();
		virtual ~ContextMenuDesc() {}
	public:

		float			contextmenu_itemHeight;
		float			contextmenu_maxMenuHeight;
		/// Sets the amount of pixel overlap sub elements are drawn over this Menu.
		float			contextmenu_subMenuOverlap;

		/**
		* Returns the class of Desc object this is.
		*/
		virtual Ogre::String getClass() { return "ContextMenuDesc"; }
		/**
		* Returns the class of Widget this desc object is meant for.
		*/
		virtual Ogre::String getWidgetClass() { return "ContextMenu"; }

		/**
		* Restore properties to default values
		*/
		virtual void resetToDefault();

		/**
		* Outlines how the desc class is written to XML and read from XML.
		*/
		virtual void serialize(SerialBase* b);
	};

	class _QuickGUIExport ContextMenu :
		public Window
	{
	public:
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
		* Determines positions of child MenuItems within the MenuPanel.
		*/
		void _updateItemPositions();

		/**
		* Hides Menu and any submenus.
		*/
		void closeMenu();
		/**
		* Closes any SubMenus that may be open.
		*/
		void closeSubMenus();
		/**
		* Creates a Child MenuImageItem.
		*/
		MenuImageItem* createImageItem(int index = -1);
		MenuImageItem* createImageItem(const Ogre::String& skin, int index = -1);
		/**
		* Creates a Child MenuItem.
		*/
		MenuItem* createMenuItem(MenuItemDesc* d, int index = -1);
		/**
		* Creates a Child MenuTextItem.
		*/
		MenuTextItem* createTextItem(const Ogre::UTFString& text, int index = -1);
		MenuTextItem* createTextItem(std::vector<TextSegment> segments, int index = -1);
		MenuTextItem* createTextItem(int index = -1);
		/**
		* Creates a Child Menu.
		*/
		Menu* createSubMenu(const Ogre::UTFString& text, int index = -1);
		Menu* createSubMenu(std::vector<TextSegment> segments, int index = -1);
		Menu* createSubMenu(int index = -1);

		/**
		* Removes an Item from the List.
		*/
		void destroyItem(unsigned int index);
		void destroyItem(MenuItem* i);

		/**
		* Returns the class name of this Widget.
		*/
		virtual Ogre::String getClass();
		/**
		* Returns the Item at the index given.  If the index is invalid,
		* NULL is returned.
		*/
		MenuItem* getItem(unsigned int index);
		/**
		* Gets the height of each ListItem within this List.
		*/
		float getItemHeight();
		/**
		* Returns the number of MenuItems this menu has.
		*/
		int getNumberOfItems();

		/**
		* Returns true if the given menu is a submenu of this menu, false otherwise.
		* NOTE: Does not have to be a direct sub menu.
		*/
		bool hasSubMenu(Menu* m);
		/**
		* Hides the Menu and any visible SubMenus.
		*/
		void hide();

		/**
		* Displays the Menu and its MenuItems.  Menu is positioned to
		* display on screen.
		*/
		void openMenu(const Point& position);
		/**
		* Finds the SubMenu and opens all Menus that lead to that Menu.
		*/
		void openSubMenu(Menu* m);

		/**
		* Sets the height of each ListItem within the List.
		*/
		void setItemHeight(float height);
		/**
		* Shows the Menu at the specified location.  Menu may be repositioned to
		* display on screen.
		*/
		void show(const Point& position);

	protected:
		ContextMenu(const Ogre::String& name);
		virtual ~ContextMenu();

		ContextMenuDesc* mDesc;

		int mAutoNameCounter;

		// Access to child MenuItems
		std::list<MenuItem*> mItems;

		// Immediate SubMenus
		std::vector<Menu*> mSubMenus;

		/**
		* Adds a child widget to this container widget.
		*/
		virtual void addChild(Widget* w);

		/**
		* Forcing all MenuItems to be the width of client dimensions.
		*/
		void onClientSizeChanged(const EventArgs& args);
		void onLoseFocus(const EventArgs& args);
		void onPositionChanged(const EventArgs& args);
		void onVisibleChanged(const EventArgs& args);

	private:
	};
}

#endif
