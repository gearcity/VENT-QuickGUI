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

#ifndef QUICKGUIMENU_H
#define QUICKGUIMENU_H

#include "QuickGUIException.h"
#include "QuickGUIMenuTextItem.h"

#include <list>

namespace QuickGUI
{
	// forward declarations
	class MenuPanel;
	class MenuImageItem;

	class _QuickGUIExport MenuDesc :
			public MenuTextItemDesc
	{
	public:
		friend class DescFactory;
	protected:
		MenuDesc();
		virtual ~MenuDesc() {}
	public:

		/// Sets if Menu will auto widen to contain the longest TextItem.
		bool			menu_autoWidenMenuPanel;
		float			menu_itemHeight;
		float			menu_maxMenuPanelHeight;
		float			menu_menuPanelWidth;
		/// Sets the amount of pixel overlap sub elements are drawn over this Menu.
		float			menu_subMenuOverlap;

		/**
		* Returns the class of Desc object this is.
		*/
		virtual Ogre::String getClass() { return "MenuDesc"; }
		/**
		* Returns the class of Widget this desc object is meant for.
		*/
		virtual Ogre::String getWidgetClass() { return "Menu"; }

		/**
		* Restore properties to default values
		*/
		virtual void resetToDefault();

		/**
		* Outlines how the desc class is written to XML and read from XML.
		*/
		virtual void serialize(SerialBase* b);
	};

	class _QuickGUIExport Menu :
		public MenuTextItem
	{
	public:
		// Skin Constants
		static const Ogre::String DEFAULT;
		static const Ogre::String DOWN;
		static const Ogre::String OVER;
		static const Ogre::String MENUPANEL;
		// Define Skin Structure
		static void registerSkinDefinition();
	public:
		friend class ToolBar;
		friend class WidgetFactory;
	public:

		/**
		* Internal function, do not use.
		*/
		virtual void _initialize(WidgetDesc* d);
		/**
		* Notifies the Widget of its GUIManager. (for component/container widgets this function is recursive)
		*/
		virtual void _setGUIManager(GUIManager* gm);
		/**
		* Notifies the Widget of its Sheet. (for component/container widgets this function is recursvie)
		*/
		virtual void _setSheet(Sheet* sheet);
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
		* Creates a Child MenuItem.
		*/
		MenuItem* createItem(MenuItemDesc* d, int index = -1);
		/**
		* Creates a Child MenuImageItem.
		*/
		MenuImageItem* createImageItem(int index = -1);
		MenuImageItem* createImageItem(const Ogre::String& skin, int index = -1);
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
		* Removes an Item from the Menu.
		*/
		void destroyItem(unsigned int index);
		void destroyItem(MenuItem* i);

		/**
		* Recursively searches through children and returns first widget found with name given.
		* NULL is returned if the widget is not found.
		*/
		virtual Widget* findWidget(const Ogre::String& name);

		/**
		* Returns true if the MenuPanel widens to match the largest TextItem, false if the width is manually set.
		*/
		bool getAutoWidenMenuPanel();
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
		* Gets the max height of the MenuPanel associated with this Menu.
		* NOTE: A max height of 0 means there are no height limits.
		*/
		float getMaxMenuPanelHeight();
		/**
		* Gets the width of the MenuPanel associated with this Menu.
		*/
		float getMenuPanelWidth();
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
		* Returns true if this Menu is the child of another Menu, false otherwise.
		*/
		bool isSubMenu();

		/**
		* Displays the Menu and its MenuItems.  Menu is positioned to
		* display on screen.
		*/
		void openMenu();
		/**
		* Finds the SubMenu and opens all Menus that lead to that Menu.
		*/
		void openSubMenu(Menu* m);

		/**
		* Builds the Widget from a ScriptDefinition or Writes the widget to a ScriptDefinition.
		*/
		virtual void serialize(SerialBase* b);
		/**
		* Set if you want the MenuPanel width to match the largest TextItem width, false if you want to manually set the MenuPanel's width.
		*/
		void setAutoWidenMenuPanel(bool autoWiden);
		/**
		* Sets the height of each ListItem within the List.
		*/
		void setItemHeight(float height);
		/**
		* Sets the max height of the MenuPanel associated with this Menu.
		* NOTE: Setting max height to 0 will remove any height limits.
		*/
		void setMaxMenuPanelHeight(float height);
		/**
		* Sets the width of the MenuPanel associated with this Menu.
		*/
		void setMenuPanelWidth(float width);
		/**
		* Sets the "type" of this widget.  For example you
		* can create several types of Button widgets: "close", "add", "fire.skill.1", etc.
		* NOTE: The type property determines what is drawn to the screen.
		*/
		virtual void setSkinType(const Ogre::String type);

	protected:
		Menu(const Ogre::String& name);
		virtual ~Menu();

		MenuDesc* mDesc;

		int mAutoNameCounter;

		// Window containing child MenuItems
		MenuPanel* mMenuPanel;
		std::list<MenuItem*> mItems;

		// Immediate SubMenus
		std::vector<Menu*> mSubMenus;

		float mMaxTextItemWidth;
		void _determineLargestTextItemWidth();
		/**
		* Adds a child widget to this container widget.
		*/
		virtual void addChild(Widget* w);
		/**
		* Updates the ContextMenu Link;
		*/
		virtual void notifyContextMenuParent(ContextMenu* m);
		/**
		* Updates the Menu Link;
		*/
		virtual void notifyMenuParent(Menu* m);
		/**
		* Updates the ToolBar Link.
		*/
		virtual void notifyToolBarParent(ToolBar* b);
		/**
		* Event handler for Left Mouse Button Down event. (Show/Hide Menu List)
		*/
		virtual void onMouseButtonDown(const EventArgs& args);
		virtual void onMouseButtonUp(const EventArgs& args);
		/**
		* Closes all SubMenus belonging to the parent Menu, and opens this menu
		*/
		virtual void onMouseEnter(const EventArgs& args);

	private:
	};
}

#endif
