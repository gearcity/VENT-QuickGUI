#include "QuickGUIMenuItem.h"
#include "QuickGUIMenu.h"
#include "QuickGUIContextMenu.h"
#include "QuickGUIToolBar.h"
#include "QuickGUISheet.h"
#include "QuickGUIManager.h"
#include "QuickGUIRoot.h"
#include "QuickGUIMenuPanel.h"

namespace QuickGUI
{
	MenuItemDesc::MenuItemDesc() :
		ToolBarItemDesc(),
		menu(NULL),
		contextMenu(NULL)
	{
		widget_resizeFromBottom = false;
		widget_resizeFromLeft = false;
		widget_resizeFromRight = false;
		widget_resizeFromTop = false;
	}

	void MenuItemDesc::serialize(SerialBase* b)
	{
		ToolBarItemDesc::serialize(b);
	}

	MenuItem::MenuItem(const Ogre::String& name) :
		ToolBarItem(name)
	{
	}

	MenuItem::~MenuItem()
	{
	}

	void MenuItem::_initialize(WidgetDesc* d)
	{
		ToolBarItem::_initialize(d);

		MenuItemDesc* mid = dynamic_cast<MenuItemDesc*>(d);
		MenuItemDesc* mDesc = dynamic_cast<MenuItemDesc*>(mWidgetDesc);

		// The only time menu should be NULL is if the Menu is created by a ToolBar, 
		// and has no parent menu.  In this case we assign the owning menu to ourself.
//		if(mid->menu == NULL)
//			mDesc->menu = dynamic_cast<Menu*>(this);
//		else
//			mDesc->menu = mid->menu;
	}

	void MenuItem::destroy()
	{
		if(isComponentOfAWidget())
			return;

		if(mParentWidget != NULL)
		{
			Ogre::String className = mParentWidget->getClass();
			if(className == "MenuPanel")
			{
				Widget* owner = dynamic_cast<MenuPanel*>(mParentWidget)->getOwner();
				Ogre::String ownerClassName = owner->getClass();
				
				if(ownerClassName == "Menu")
					dynamic_cast<Menu*>(owner)->destroyItem(this);
			}
			else if(className == "ContextMenu")
				dynamic_cast<ContextMenu*>(mParentWidget)->destroyItem(this);
		}
		else
		{
			if(mWidgetDesc->sheet != NULL)
				mWidgetDesc->sheet->mFreeList.push_back(this);
			else
				Root::getSingleton().mGUIManagers.begin()->second->mFreeList.push_back(this);
		}
	}

	ContextMenu* MenuItem::getContextMenu()
	{
		return dynamic_cast<MenuItemDesc*>(mWidgetDesc)->contextMenu;
	}

	Menu* MenuItem::getMenu()
	{
		return dynamic_cast<MenuItemDesc*>(mWidgetDesc)->menu;
	}

	ToolBar* MenuItem::getToolBar()
	{
		return dynamic_cast<MenuItemDesc*>(mWidgetDesc)->toolBar;
	}

	bool MenuItem::isMenuItem()
	{
		return true;
	}

	void MenuItem::notifyContextMenuParent(ContextMenu* m)
	{
		dynamic_cast<MenuItemDesc*>(mWidgetDesc)->contextMenu = m;
	}

	void MenuItem::notifyMenuParent(Menu* m)
	{
		dynamic_cast<MenuItemDesc*>(mWidgetDesc)->menu = m;
	}

	void MenuItem::notifyToolBarParent(ToolBar* b)
	{
		dynamic_cast<MenuItemDesc*>(mWidgetDesc)->toolBar = b;
	}

	void MenuItem::setVisible(bool visible)
	{
		ToolBarItem::setVisible(visible);

		MenuItemDesc* mid = dynamic_cast<MenuItemDesc*>(mWidgetDesc);

		// Its important to check menu before contextMenu, as they both may be populated,
		// in the case of a MenuItem on a Menu within a ContextMenu.
		if(mid->menu != NULL)
			mid->menu->_updateItemPositions();
		else if(mid->contextMenu != NULL)
			mid->contextMenu->_updateItemPositions();
		else if(mid->toolBar != NULL)
			mid->toolBar->_updateItemPositions();
	}
}
