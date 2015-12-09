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

#ifndef QUICKGUITOOLBAR_H
#define QUICKGUITOOLBAR_H

#include "QuickGUIContainerWidget.h"
#include "QuickGUIToolBarEnums.h"
#include "QuickGUIToolBarItem.h"
#include "QuickGUIText.h"

#include <vector>

namespace QuickGUI
{
	// forward declarations
	class Menu;

	class _QuickGUIExport ToolBarDesc :
			public ContainerWidgetDesc
	{
	public:
		friend class DescFactory;
	protected:
		ToolBarDesc();
		virtual ~ToolBarDesc() {}
	public:

		ToolBarItemLayout toolbar_itemLayout;

		/**
		* Returns the class of Desc object this is.
		*/
		virtual Ogre::String getClass() { return "ToolBarDesc"; }
		/**
		* Returns the class of Widget this desc object is meant for.
		*/
		virtual Ogre::String getWidgetClass() { return "ToolBar"; }

		/**
		* Restore properties to default values
		*/
		virtual void resetToDefault();

		/**
		* Outlines how the desc class is written to XML and read from XML.
		*/
		virtual void serialize(SerialBase* b);
	};

	class _QuickGUIExport ToolBar :
		public ContainerWidget
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
		/*
		* Updates Item Indices and Positions
		*/
		void _updateItemPositions();

		/**
		* Closes all Menus that belong to this ToolBar.
		*/
		void closeMenus();
		/**
		* Creates a Menu.  Position and orientation depends on
		* toolbar_itemLayout and ToolBar orientation.
		*/
		Menu* createMenu(const Ogre::UTFString& text, int index = -1);
		Menu* createMenu(std::vector<TextSegment> segments, int index = -1);
		Menu* createMenu(int index = -1);
		/**
		* Creates a ToolBar item.  Position and orientation depends on
		* toolbar_itemLayout and ToolBar orientation.
		*/
		ToolBarItem* createToolBarItem(ToolBarItemDesc* d, int index = -1);

		/**
		* Removes the item from the ToolBar and destroys it.
		*/
		void destroyItem(ToolBarItem* i);

		/**
		* Returns the class name of this Widget.
		*/
		virtual Ogre::String getClass();
		/**
		* Returns the number of ToolBarItems in the ToolBar.
		*/
		int getNumberOfItems();
		/**
		* Get the orientation of the ToolBar.
		*/
		ToolBarOrientation getOrientation();

		/**
		* Returns true if a menu is open, false otherwise.
		*/
		bool isMenuOpened();

		/**
		* Opens the Menu and any Menus that are parents of the given menu.
		*/
		void openMenu(Menu* m);

		/**
		* Recalculate Screen and client widget_dimensions and force a redrawing of the widget.
		*/
		virtual void updateTexturePosition();

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
		ToolBar(const Ogre::String& name);
		virtual ~ToolBar();

		ToolBarOrientation mOrientation;
		ToolBarState mCurrentState;

		ToolBarDesc* mDesc;

		std::list<ToolBarItem*> mItems;

		std::vector<Menu*> mMenus;
		bool mMenuOpened;

		/**
		* Adds a child widget to this container widget.
		*/
		virtual void addChild(Widget* w);

		/**
		* Outlines how the widget is drawn to the current render target
		*/
		virtual void onDraw();

	private:
	};
}

#endif
