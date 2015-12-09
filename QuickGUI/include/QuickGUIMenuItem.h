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
