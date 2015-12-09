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

#ifndef QUICKGUIMENUITEM_H
#define QUICKGUIMENUITEM_H

#include "QuickGUIToolBarItem.h"

namespace QuickGUI
{
	// forward declaration
	class ContextMenu;
	class Menu;

	class _QuickGUIExport MenuItemDesc :
			public ToolBarItemDesc
	{
	public:
		friend class DescFactory;
	protected:
		MenuItemDesc();
		virtual ~MenuItemDesc() {}
	public:

		// Keep track of the menu this item belongs to. Due to qgui design,
		// the menuItem's parent is the MenuPanel.
		Menu* menu;

		// MenuItems originate from ToolBars or ContextMenus.  Keep track of
		// contextMenu, if applicable.
		ContextMenu* contextMenu;

		/**
		* Outlines how the desc class is written to XML and read from XML.
		*/
		virtual void serialize(SerialBase* b);
	};

	class _QuickGUIExport MenuItem :
		public ToolBarItem
	{
	public:
		// Gives ContextMenu access to reposition MenuItems
		friend class ContextMenu;
		friend class ToolBar;
		friend class Menu;
		friend class MenuPanel;
	public:

		/**
		* Internal function, do not use.
		*/
		virtual void _initialize(WidgetDesc* d);

		/**
		* Queues this widget for destruction, likely to happen in the next frame, before drawing occurs.
		* NOTE: Widgets that are components of another widget cannot be destroyed using this API.
		*/
		virtual void destroy();

		/**
		* Returns the ContextMenu this MenuItem belongs to, or NULL if
		* this MenuItem does not belong to a ContextMenu.
		*/
		ContextMenu* getContextMenu();
		/**
		* Returns the Menu this MenuItem belongs to.
		* NOTE: This MenuItem could be a Menu, and if the Menu is directly connected to a ToolBar,
		* its parent Menu will be itself.
		*/
		Menu* getMenu();
		/**
		* Returns the ToolBar this MenuItem belongs to, or NULL if
		* this MenuItem does not belong to a ToolBar.
		*/
		ToolBar* getToolBar();

		/**
		* Returns true if this widget is a menu item, false otherwise.
		*/
		virtual bool isMenuItem();

		/**
		* Sets whether or not the widget gets drawn.
		*/
		virtual void setVisible(bool visible);

	protected:
		MenuItem(const Ogre::String& name);
		virtual ~MenuItem();

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

	private:
	};
}

#endif
