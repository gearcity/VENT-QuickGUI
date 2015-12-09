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

#include "QuickGUIMenu.h"
#include "QuickGUISheet.h"
#include "QuickGUIManager.h"
#include "QuickGUIMenuPanel.h"
#include "QuickGUIMenuImageItem.h"
#include "QuickGUIScriptReader.h"
#include "QuickGUISkinDefinitionManager.h"
#include "QuickGUIFactoryManager.h"
#include "QuickGUISkinDefinition.h"
#include "QuickGUIContextMenu.h"
#include "QuickGUIToolBar.h"
#include "QuickGUIDescManager.h"
#include "QuickGUIWidgetFactory.h"
#include "QuickGUIRoot.h"

namespace QuickGUI
{
	const Ogre::String Menu::DEFAULT = "default";
	const Ogre::String Menu::DOWN = "down";
	const Ogre::String Menu::OVER = "over";
	const Ogre::String Menu::MENUPANEL = "menupanel";

	void Menu::registerSkinDefinition()
	{
		SkinDefinition* d = OGRE_NEW_T(SkinDefinition,Ogre::MEMCATEGORY_GENERAL)("Menu");
		d->defineSkinElement(DEFAULT);
		d->defineSkinElement(OVER);
		d->defineSkinElement(DOWN);
		d->defineSkinReference(MENUPANEL,"MenuPanel");
		d->definitionComplete();

		SkinDefinitionManager::getSingleton().registerSkinDefinition("Menu",d);
	}

	MenuDesc::MenuDesc() :
		MenuTextItemDesc()
	{
		resetToDefault();
	}

	void MenuDesc::resetToDefault()
	{
		MenuTextItemDesc::resetToDefault();

		menu = NULL;
		menu_autoWidenMenuPanel = true;
		menu_itemHeight = 25;
		menu_maxMenuPanelHeight = 0;
		menu_menuPanelWidth = 100;
		menu_subMenuOverlap = 2;
	}

	void MenuDesc::serialize(SerialBase* b)
	{
		MenuTextItemDesc::serialize(b);

		// Retrieve default values to supply to the serial reader/writer.
		// The reader uses the default value if the given property does not exist.
		// The writer does not write out the given property if it has the same value as the default value.
		MenuDesc* defaultValues = DescManager::getSingleton().createDesc<MenuDesc>(getClass(),"temp");
		defaultValues->resetToDefault();

		b->IO("AutoWidenMenuPanel", &menu_autoWidenMenuPanel,	defaultValues->menu_autoWidenMenuPanel);
		b->IO("ItemHeight",			&menu_itemHeight,			defaultValues->menu_itemHeight);
		b->IO("MaxMenuHeight",		&menu_maxMenuPanelHeight,	defaultValues->menu_maxMenuPanelHeight);
		b->IO("MenuWidth",			&menu_menuPanelWidth,		defaultValues->menu_menuPanelWidth);
		b->IO("SubMenuOverlap",		&menu_subMenuOverlap,		defaultValues->menu_subMenuOverlap);

		DescManager::getSingleton().destroyDesc(defaultValues);
	}

	Menu::Menu(const Ogre::String& name) :
		MenuTextItem(name),
		mMenuPanel(NULL),
		mAutoNameCounter(0),
		mMaxTextItemWidth(0)
	{
	}

	Menu::~Menu()
	{
		// If our MenuPanel is not attached to any Sheet, delete it
		if(mWidgetDesc->sheet == NULL)
			FactoryManager::getSingleton().getWidgetFactory()->destroyInstance(mMenuPanel);
		// Else it is attached to Sheet
		else
		{
			// If Sheet is not in its destructor, remove MenuPanel and Destroy it
			if(!mWidgetDesc->sheet->mDeleting)
				mWidgetDesc->sheet->destroyWindow(mMenuPanel->getName());
		}
	}

	void Menu::_determineLargestTextItemWidth()
	{
		mMaxTextItemWidth = 0;
		float textWidth = 0;
		MenuTextItem* mti = NULL;
		for( std::list<MenuItem*>::iterator it = mItems.begin(); it != mItems.end(); ++it )
		{
			mti = dynamic_cast<MenuTextItem*>((*it));
			if(mti != NULL)
			{
				textWidth = mti->getTextWidth();

				if(textWidth > mMaxTextItemWidth)
					mMaxTextItemWidth = textWidth;
			}
		}
	}

	void Menu::_initialize(WidgetDesc* d)
	{
		MenuTextItem::_initialize(d);

		mDesc = dynamic_cast<MenuDesc*>(mWidgetDesc);

		MenuDesc* md = dynamic_cast<MenuDesc*>(d);

		// Copy over all properties from desc param
		mDesc->menu_autoWidenMenuPanel = md->menu_autoWidenMenuPanel;
		mDesc->menu_maxMenuPanelHeight = md->menu_maxMenuPanelHeight;
		mDesc->menu_menuPanelWidth = md->menu_menuPanelWidth;
		mDesc->menu_subMenuOverlap = md->menu_subMenuOverlap;

		// Create our Menu List Window
		MenuPanelDesc* lpd = DescManager::getSingletonPtr()->getDefaultMenuPanelDesc();
		lpd->resetToDefault();
		lpd->widget_name = mWidgetDesc->widget_name + ".MenuPanel";
		lpd->widget_dimensions = Rect(0,0,mDesc->menu_menuPanelWidth,1);
		lpd->widget_visible = false;
		lpd->menupanel_owner = this;
		lpd->menupanel_maxHeight = mDesc->menu_maxMenuPanelHeight;
		mMenuPanel = dynamic_cast<MenuPanel*>(_createWidget(lpd));

		setSkinType(d->widget_skinTypeName);
	}

	void Menu::_setGUIManager(GUIManager* gm)
	{
		MenuTextItem::_setGUIManager(gm);

		if(mMenuPanel != NULL)
			mMenuPanel->_setGUIManager(gm);

		for(std::list<MenuItem*>::iterator it = mItems.begin(); it != mItems.end(); ++it)
			(*it)->_setGUIManager(gm);
	}

	void Menu::_setSheet(Sheet* sheet)
	{
		bool changingSheets = (mWidgetDesc->sheet != sheet);

		// If we are changing sheets, detach MenuPanel from previous sheet
		if((mWidgetDesc->sheet != NULL) && changingSheets)
			mWidgetDesc->sheet->removeWindow(mMenuPanel);

		MenuTextItem::_setSheet(sheet);

		// If new sheet is valid, and this MenuPanel hasn't been added to it, add it
		if((sheet != NULL) && !(sheet->hasWindow(mMenuPanel->getName())) && (mMenuPanel->getSheet() == NULL))
			sheet->addWindow(mMenuPanel);

		for(std::list<MenuItem*>::iterator it = mItems.begin(); it != mItems.end(); ++it)
			(*it)->_setSheet(sheet);
	}

	void Menu::_updateItemPositions()
	{
		float y = 0;
		for(std::list<MenuItem*>::iterator it = mItems.begin(); it != mItems.end(); ++it)
		{
			(*it)->setPosition(Point(0,y));
			(*it)->setHeight(mDesc->menu_itemHeight);

			if((*it)->getVisible())
				y += mDesc->menu_itemHeight;
		}

		if(mMenuPanel != NULL)
			mMenuPanel->_adjustHeight();
	}

	bool Menu::getAutoWidenMenuPanel()
	{
		return mDesc->menu_autoWidenMenuPanel;
	}

	Ogre::String Menu::getClass()
	{
		return "Menu";
	}

	void Menu::addChild(Widget* w)
	{
		if(!w->isMenuItem())
			throw Exception(Exception::ERR_INVALID_CHILD,"Cannot add non-MenuItem to a menu!","Menu::addChild");

		// Set link to parent Menu
		MenuItem* i = dynamic_cast<MenuItem*>(w);
		i->notifyMenuParent(this);

		// Set link to parent ToolBar
		i->notifyToolBarParent(mDesc->toolBar);

		// Set link to parent ContextMenu
		i->notifyContextMenuParent(mDesc->contextMenu);

		i->_setGUIManager(mDesc->guiManager);
		i->_setSheet(mDesc->sheet);
		i->setHeight(mDesc->menu_itemHeight);

		// We cannot add the widget as a child, the texture position and drawing will be incorrect.
		int itemIndex = i->getIndex();

		// Adding Item to End of List
		if((itemIndex < 0) || (itemIndex >= static_cast<int>(mItems.size())))
		{
			// Update Index
			i->setIndex(static_cast<int>(mItems.size()));
			
			// Update Position
			if(!mItems.empty())
			{
				Point p = mItems.back()->getPosition();
				p.y += i->getSize().height;
				i->setPosition(p);
			}

			mItems.push_back(i);
		}
		// Inserting Item into the List
		else
		{
			int count = 0;
			for(std::list<MenuItem*>::iterator it = mItems.begin(); it != mItems.end(); ++it)
			{
				if(count == itemIndex)
				{
					// Insert Item into List.  All items after this will need to have
					// index and position updated.

					Point p = (*it)->getPosition();

					mItems.insert(it,i);
					i->setPosition(p);

					break;
				}

				++count;
			}

			count = 0;
			Point p;
			for(std::list<MenuItem*>::iterator it = mItems.begin(); it != mItems.end(); ++it)
			{
				// Update Index
				(*it)->setIndex(count);
				// Update Position
				(*it)->setPosition(p);

				p.y += (*it)->getHeight();

				++count;
			}
		}
		
		// Add to the windows list of MenuItems
		mMenuPanel->addWidget(i);

		if(i->getClass() == "Menu")
			mSubMenus.push_back(dynamic_cast<Menu*>(i));
	}

	void Menu::closeMenu()
	{
		// Close all SubMenus
		for(std::vector<Menu*>::iterator it = mSubMenus.begin(); it != mSubMenus.end(); ++it)
			(*it)->closeMenu();

		mMenuPanel->setVisible(false);
	}

	void Menu::closeSubMenus()
	{
		for(std::vector<Menu*>::iterator it = mSubMenus.begin(); it != mSubMenus.end(); ++it)
		{
			(*it)->closeMenu();
		}
	}

	MenuItem* Menu::createItem(MenuItemDesc* d, int index)
	{
		if(d->widget_name == "")
		{
			d->widget_name = getName() + ".AutoName.MenuItem." + Ogre::StringConverter::toString(mAutoNameCounter);
			++mAutoNameCounter;
		}

		// Make sure new MenuItem will maintain link to owner ToolBar
		d->toolBar = mDesc->toolBar;

		// Make sure new MenuItem will maintain link to owner ContextMenu
		d->contextMenu = mDesc->contextMenu;

		// Make sure new MenuItem will maintain link to owner Menu
		d->menu = this;

		// Determine position of MenuItem
		d->widget_dimensions.size.width = mMenuPanel->getClientDimensions().size.width;
		d->widget_dimensions.size.height = mDesc->menu_itemHeight;
		d->index = index;
		
		MenuItem* newMenuItem = dynamic_cast<MenuItem*>(_createWidget(d));
		addChild(newMenuItem);

		if(mDesc->menu_autoWidenMenuPanel)
		{
			MenuTextItem* mti = dynamic_cast<MenuTextItem*>(newMenuItem);
			if(mti != NULL)
			{
				float textWidth = mti->getTextWidth();
				if(textWidth > mMaxTextItemWidth)
				{
					mMaxTextItemWidth = textWidth;

					if(mMenuPanel != NULL)
						mMenuPanel->setWidth(mMenuPanel->getLeftRightBorderWidth() + mMaxTextItemWidth + 5);
				}
			}
		}

		return newMenuItem;
	}

	MenuImageItem* Menu::createImageItem(int index)
	{
		MenuImageItemDesc* d = DescManager::getSingletonPtr()->getDefaultMenuImageItemDesc();
		d->resetToDefault();
		
		MenuImageItem* i = dynamic_cast<MenuImageItem*>(createItem(d,index));

		d->resetToDefault();

		return i;
	}

	MenuImageItem* Menu::createImageItem(const Ogre::String& skin, int index)
	{
		MenuImageItemDesc* d = DescManager::getSingletonPtr()->getDefaultMenuImageItemDesc();
		d->resetToDefault();
		d->widget_skinTypeName = skin;
		
		MenuImageItem* i = dynamic_cast<MenuImageItem*>(createItem(d,index));

		d->resetToDefault();

		return i;
	}

	MenuTextItem* Menu::createTextItem(const Ogre::UTFString& text, int index)
	{
		MenuTextItemDesc* d = DescManager::getSingletonPtr()->getDefaultMenuTextItemDesc();
		d->resetToDefault();
		d->textDesc.segments.push_back(TextSegment(text));

		MenuTextItem* i = dynamic_cast<MenuTextItem*>(createItem(d,index));

		d->resetToDefault();

		return i;
	}

	MenuTextItem* Menu::createTextItem(std::vector<TextSegment> segments, int index)
	{
		MenuTextItemDesc* d = DescManager::getSingletonPtr()->getDefaultMenuTextItemDesc();
		d->resetToDefault();
		d->textDesc.segments = segments;

		MenuTextItem* i = dynamic_cast<MenuTextItem*>(createItem(d,index));

		d->resetToDefault();

		return i;
	}

	MenuTextItem* Menu::createTextItem(int index)
	{
		return createTextItem("MenuTextItem",index);
	}

	Menu* Menu::createSubMenu(const Ogre::UTFString& text, int index)
	{
		MenuDesc* d = DescManager::getSingletonPtr()->getDefaultMenuDesc();
		d->resetToDefault();
		d->textDesc.segments.push_back(TextSegment(text));

		Menu* m = dynamic_cast<Menu*>(createItem(d,index));

		d->resetToDefault();

		return m;
	}

	Menu* Menu::createSubMenu(std::vector<TextSegment> segments, int index)
	{
		MenuDesc* d = DescManager::getSingletonPtr()->getDefaultMenuDesc();
		d->resetToDefault();
		d->textDesc.segments = segments;

		Menu* m = dynamic_cast<Menu*>(createItem(d,index));

		d->resetToDefault();

		return m;
	}

	Menu* Menu::createSubMenu(int index)
	{
		return createSubMenu("SubMenu",index);
	}

	void Menu::destroyItem(unsigned int index)
	{
		WidgetFactory* f = FactoryManager::getSingleton().getWidgetFactory();

		bool itemRemovedFromList = false;
		unsigned int count = 0;
		for(std::list<MenuItem*>::iterator it = mItems.begin(); it != mItems.end(); ++it)
		{
			if(count == index)
			{
				mMenuPanel->removeWidget((*it));

				if(mDesc->sheet != NULL)
					mDesc->sheet->mFreeList.push_back((*it));
				else
					Root::getSingleton().mGUIManagers.begin()->second->mFreeList.push_back((*it));
				mItems.erase(it);

				itemRemovedFromList = true;

				break;
			}

			++count;
		}

		if(!itemRemovedFromList)
			return;

		float yPos = 0;
		count = 0;
		for(std::list<MenuItem*>::iterator it = mItems.begin(); it != mItems.end(); ++it)
		{
			// Update index
			(*it)->setIndex(count);

			// Update Position
			Point p = (*it)->getPosition();
			p.y = yPos;
			(*it)->setPosition(p);

			// Store position of next item
			yPos += (*it)->getSize().height;

			++count;
		}

		mMenuPanel->_adjustHeight();

		_determineLargestTextItemWidth();

		if(mDesc->menu_autoWidenMenuPanel && (mMenuPanel != NULL))
			mMenuPanel->setWidth(mMenuPanel->getLeftRightBorderWidth() + mMaxTextItemWidth + 5);

		redraw();
	}

	void Menu::destroyItem(MenuItem* i)
	{
		destroyItem(i->getIndex());
	}

	Widget* Menu::findWidget(const Ogre::String& name)
	{
		if(getName() == name)
			return this;

		Widget* w = NULL;

		// Search through menu items
		for(std::list<MenuItem*>::iterator it = mItems.begin(); it != mItems.end(); ++it)
		{
			w = (*it)->findWidget(name);
			if(w != NULL)
				return w;
		}

		return mMenuPanel->findWidget(name);
	}

	MenuItem* Menu::getItem(unsigned int index)
	{
		if(index >= mItems.size())
			return NULL;

		unsigned int count = 0;
		for(std::list<MenuItem*>::iterator it = mItems.begin(); it != mItems.end(); ++it)
		{
			if(count == index)
				return (*it);

			++count;
		}

		return NULL;
	}

	float Menu::getItemHeight()
	{
		return mDesc->menu_itemHeight;
	}

	float Menu::getMaxMenuPanelHeight()
	{
		return mDesc->menu_maxMenuPanelHeight;
	}

	float Menu::getMenuPanelWidth()
	{
		return mDesc->menu_menuPanelWidth;
	}

	int Menu::getNumberOfItems()
	{
		return static_cast<int>(mItems.size());
	}

	bool Menu::hasSubMenu(Menu* m)
	{
		// Check if menu is an immediate sub menu
		if(this == m)
			return true;

		// recurse submenus to see if menu is a submenu
		for(std::vector<Menu*>::iterator it = mSubMenus.begin(); it != mSubMenus.end(); ++it)
		{
			if( (*it)->hasSubMenu(m) )
				return true;
		}

		return false;
	}

	bool Menu::isSubMenu()
	{
		if(mParentWidget == NULL)
			return false;

		return (mParentWidget->getClass() != "ToolBar");
	}

	void Menu::notifyContextMenuParent(ContextMenu* m)
	{
		MenuItem::notifyContextMenuParent(m);

		for(std::list<MenuItem*>::iterator it = mItems.begin(); it != mItems.end(); ++it)
			(*it)->notifyContextMenuParent(m);
	}

	void Menu::notifyMenuParent(Menu* m)
	{
		MenuItem::notifyMenuParent(m);

		for(std::list<MenuItem*>::iterator it = mItems.begin(); it != mItems.end(); ++it)
			(*it)->notifyMenuParent(this);
	}

	void Menu::notifyToolBarParent(ToolBar* b)
	{
		MenuItem::notifyToolBarParent(b);

		for(std::list<MenuItem*>::iterator it = mItems.begin(); it != mItems.end(); ++it)
			(*it)->notifyToolBarParent(b);
	}

	void Menu::onMouseButtonDown(const EventArgs& args)
	{
		const MouseEventArgs& mea = dynamic_cast<const MouseEventArgs&>(args);

		if(mea.button == MB_Left)
		{
			if(mParentWidget == NULL)
				throw Exception(Exception::ERR_INVALID_STATE,"Menu is not attached to any widget! (Parent is NULL)","Menu::onLeftMouseDown");

			if(mParentWidget->getClass() == "ToolBar")
			{
				if(mMenuPanel->getVisible())
					mDesc->toolBar->closeMenus();
				else
					mDesc->toolBar->openMenu(this);
			}
			else
			{
				if(mMenuPanel->getVisible())
					closeMenu();
				else
				{
					if(mDesc->toolBar != NULL)
						mDesc->toolBar->openMenu(this);
					else if(mDesc->contextMenu != NULL)
						mDesc->contextMenu->openSubMenu(this);
				}
			}
		}
	}

	void Menu::onMouseButtonUp(const EventArgs& args)
	{
		// Purposefully left blank. (overrides MenuTextItem::onMouseButtonUp, which is called as an event handler)
	}

	void Menu::onMouseEnter(const EventArgs& args)
	{
		MenuTextItem::onMouseEnter(args);

		if(mParentWidget == NULL)
			throw Exception(Exception::ERR_INVALID_STATE,"Menu is not attached to any widget! (Parent is NULL)","Menu::onMouseEnter");

		// For standard menus, go through toolBar to open the right menu
		if((mDesc->toolBar != NULL) && mDesc->toolBar->isMenuOpened())
			mDesc->toolBar->openMenu(this);
		else if(mDesc->contextMenu != NULL)
			mDesc->contextMenu->openSubMenu(this);
	}

	void Menu::openMenu()
	{
		// Close all SubMenus
		for(std::vector<Menu*>::iterator it = mSubMenus.begin(); it != mSubMenus.end(); ++it)
			(*it)->closeMenu();

		Point p = getScreenPosition();

		if(mParentWidget->getClass() == "ToolBar")
		{
			ToolBar* tb = mDesc->toolBar;
			if(tb == NULL)
				throw Exception(Exception::ERR_INVALID_STATE,"Parent ToolBar reference is NULL!","Menu::openMenu");

			if(tb->getOrientation() == TOOLBAR_ORIENTATION_HORIZONTAL)
			{
				// Position the MenuList above or below the MenuTextItem - below is desired
				if((getScreenPosition().y + mDesc->widget_dimensions.size.height + mMenuPanel->getSize().height) > mDesc->guiManager->getViewport()->getActualHeight())
					p.y = mParentWidget->getScreenPosition().y - mMenuPanel->getSize().height;
				else
					p.y = mParentWidget->getScreenPosition().y + mParentWidget->getSize().height;
			}
			else // orientation is TOOLBAR_ORIENTATION_VERTICAL
			{
				// Position the MenuList to the left or right of the MenuTextItem - right is desired
				if((getScreenPosition().x + mDesc->widget_dimensions.size.width + mMenuPanel->getSize().width) > mDesc->guiManager->getViewport()->getActualWidth())
					p.x = mParentWidget->getScreenPosition().x - mMenuPanel->getSize().width;
				else
					p.x = mParentWidget->getScreenPosition().x + mParentWidget->getSize().width;
			}
		}
		else // parent class is "MenuPanel", which means this menu is a SubMenu of another Menu, or menu of a ContextMenu
		{
			// Position the MenuList to the left or right of the MenuTextItem - right is desired
			if((getScreenPosition().x + getSize().width + mMenuPanel->getSize().width) > mDesc->guiManager->getViewport()->getActualWidth())
				p.x = mParentWidget->getScreenPosition().x - mMenuPanel->getSize().width + mDesc->menu_subMenuOverlap;
			else
				p.x = mParentWidget->getScreenPosition().x + mParentWidget->getSize().width - mDesc->menu_subMenuOverlap;

			p.y = getScreenPosition().y - mMenuPanel->mSkinElement->getBorderThickness(BORDER_TOP);
		}

		mMenuPanel->setPosition(p);
		mMenuPanel->setVisible(true);
	}

	void Menu::openSubMenu(Menu* m)
	{
		if( this == m )
		{
			openMenu();
			return;
		}

		for(std::vector<Menu*>::iterator it = mSubMenus.begin(); it != mSubMenus.end(); ++it)
		{
			if((*it)->hasSubMenu(m))
			{
				openMenu();
				(*it)->openSubMenu(m);
			}
			else
				(*it)->closeMenu();
		}
	}

	void Menu::serialize(SerialBase* b)
	{
		// Create Desc object if its not already created.
		_createDescObject();

		if(!mWidgetDesc->widget_serialize)
			return;

		b->begin(getClass(),getName());

		mWidgetDesc->serialize(b);

		if(b->isSerialReader())
			// Apply Desc properties
			_initialize(mWidgetDesc);

		if(b->begin("Child","Widgets"))
		{
			// If we are reading, we need to create and populate widgets
			if(b->isSerialReader())
			{
				std::list<ScriptDefinition*> defList = b->getCurrentDefinition()->getDefinitions();
				for(std::list<ScriptDefinition*>::iterator it = defList.begin(); it != defList.end(); ++it)
				{
					// Create Empty Widget, supplying class name and widget name from script
					Widget* newWidget = FactoryManager::getSingleton().getWidgetFactory()->createInstance<Widget>((*it)->getType(),(*it)->getID());

					// Populate Desc object from Script Text, and initialize widget
					newWidget->serialize(b);

					// Add as child Widget
					addChild(newWidget);
				}
			}
			// If we are writing, we need to serialize children
			else
			{
				for(std::list<MenuItem*>::iterator it = mItems.begin(); it != mItems.end(); ++it)
				{
					(*it)->serialize(b);
				}
			}
			b->end();
		}

		b->end();
	}

	void Menu::setAutoWidenMenuPanel(bool autoWiden)
	{
		mDesc->menu_autoWidenMenuPanel = autoWiden;

		if(autoWiden)
		{
			_determineLargestTextItemWidth();

			if(mMenuPanel != NULL)
				mMenuPanel->setWidth(mMenuPanel->getLeftRightBorderWidth() + mMaxTextItemWidth + 5);
		}
	}

	void Menu::setItemHeight(float height)
	{
		mDesc->menu_itemHeight = height;

		_updateItemPositions();
	}

	void Menu::setMaxMenuPanelHeight(float height)
	{
		mDesc->menu_maxMenuPanelHeight = Ogre::Math::Floor(height + 0.5);

		if(mMenuPanel != NULL)
			mMenuPanel->setMaxHeight(mDesc->menu_maxMenuPanelHeight);
	}

	void Menu::setMenuPanelWidth(float width)
	{
		mDesc->menu_autoWidenMenuPanel = false;

		mDesc->menu_menuPanelWidth = Ogre::Math::Floor(width + 0.5);

		if(mMenuPanel != NULL)
			mMenuPanel->setWidth(mDesc->menu_menuPanelWidth);
	}

	void Menu::setSkinType(const Ogre::String type)
	{
		Widget::setSkinType(type);

		for(std::map<Ogre::String,Widget*>::iterator it = mComponents.begin(); it != mComponents.end(); ++it)
			(*it).second->setSkinType(mSkinType->getSkinReference((*it).first)->typeName);

		if(mMenuPanel != NULL)
			mMenuPanel->setSkinType(mSkinType->getSkinReference(MENUPANEL)->typeName);
	}
}
