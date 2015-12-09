#include "QuickGUIContextMenu.h"
#include "QuickGUISheet.h"
#include "QuickGUIManager.h"
#include "QuickGUISkinDefinitionManager.h"
#include "QuickGUIFactoryManager.h"
#include "QuickGUISkinDefinition.h"
#include "QuickGUIMenuItem.h"
#include "QuickGUIMenuImageItem.h"
#include "QuickGUIMenuTextItem.h"
#include "QuickGUIMenu.h"
#include "QuickGUITitleBar.h"
#include "QuickGUIDescManager.h"
#include "QuickGUIWidgetFactory.h"
#include "QuickGUIRoot.h"
#include "QuickGUIManager.h"

namespace QuickGUI
{
	void ContextMenu::registerSkinDefinition()
	{
		SkinDefinition* d = OGRE_NEW_T(SkinDefinition,Ogre::MEMCATEGORY_GENERAL)("ContextMenu");
		d->defineSkinElement(BACKGROUND);
		d->defineComponent(TITLEBAR);
		d->defineComponent(HSCROLLBAR);
		d->defineComponent(VSCROLLBAR);
		d->definitionComplete();

		SkinDefinitionManager::getSingleton().registerSkinDefinition("ContextMenu",d);
	}

	ContextMenuDesc::ContextMenuDesc() :
		WindowDesc()
	{
		resetToDefault();
	}

	void ContextMenuDesc::resetToDefault()
	{
		WindowDesc::resetToDefault();

		contextmenu_itemHeight = 25;
		contextmenu_maxMenuHeight = 0;
		contextmenu_subMenuOverlap = 2;

		widget_dimensions.size = Size(100,25);
		widget_resizeFromBottom = false;
		widget_resizeFromLeft = false;
		widget_resizeFromRight = false;
		widget_resizeFromTop = false;
		widget_visible = false;
	}

	void ContextMenuDesc::serialize(SerialBase* b)
	{
		WindowDesc::serialize(b);

		b->IO("ItemHeight",&contextmenu_itemHeight);
		b->IO("MaxMenuHeight",&contextmenu_maxMenuHeight);
		b->IO("SubMenuOverlap",&contextmenu_subMenuOverlap);
	}

	ContextMenu::ContextMenu(const Ogre::String& name) :
		Window(name),
		mAutoNameCounter(0)
	{
		addWidgetEventHandler(WIDGET_EVENT_CLIENTSIZE_CHANGED,&ContextMenu::onClientSizeChanged,this);
		addWindowEventHandler(WINDOW_EVENT_FOCUS_LOST,&ContextMenu::onLoseFocus,this);
		addWidgetEventHandler(WIDGET_EVENT_POSITION_CHANGED,&ContextMenu::onPositionChanged,this);
		addWidgetEventHandler(WIDGET_EVENT_VISIBLE_CHANGED,&ContextMenu::onVisibleChanged,this);
	}

	ContextMenu::~ContextMenu()
	{
	}

	void ContextMenu::_initialize(WidgetDesc* d)
	{
		d->widget_visible = false;
		d->widget_resizeFromBottom = false;
		d->widget_resizeFromLeft = false;
		d->widget_resizeFromRight = false;
		d->widget_resizeFromTop = false;

		Window::_initialize(d);

		setSkinType(d->widget_skinTypeName);

		mDesc = dynamic_cast<ContextMenuDesc*>(mWidgetDesc);

		ContextMenuDesc* cmd = dynamic_cast<ContextMenuDesc*>(d);

		if(cmd->window_titleBar)
			Window::setHeight(
				mTitleBar->getHeight() + 
				mSkinElement->getBorderThickness(BORDER_TOP) +
				mSkinElement->getBorderThickness(BORDER_BOTTOM));
		else
			Window::setHeight(
				mSkinElement->getBorderThickness(BORDER_TOP) +
				mSkinElement->getBorderThickness(BORDER_BOTTOM));

		// Copy over all properties from desc param
		mDesc->contextmenu_itemHeight = cmd->contextmenu_itemHeight;
		mDesc->contextmenu_maxMenuHeight = cmd->contextmenu_maxMenuHeight;
		mDesc->contextmenu_subMenuOverlap = cmd->contextmenu_subMenuOverlap;
	}

	void ContextMenu::_updateItemPositions()
	{
		int y = 0;
		for(std::list<MenuItem*>::iterator it = mItems.begin(); it != mItems.end(); ++it)
		{
			(*it)->setPosition(Point(0,y));
			(*it)->setHeight(mDesc->contextmenu_itemHeight);

			if((*it)->getVisible())
				y += mDesc->contextmenu_itemHeight;
		}
	}

	Ogre::String ContextMenu::getClass()
	{
		return "ContextMenu";
	}

	void ContextMenu::addChild(Widget* w)
	{
		if(!w->isMenuItem())
			throw Exception(Exception::ERR_INVALID_CHILD,"Cannot add non-MenuItem to a menu!","Menu::addChild");

		ContainerWidget::addChild(w);

		// Set link to parent Menu
		MenuItem* i = dynamic_cast<MenuItem*>(w);

		// Set link to parent ContextMenu
		i->notifyContextMenuParent(this);

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

		if(i->getClass() == "Menu")
			mSubMenus.push_back(dynamic_cast<Menu*>(i));

		Window::setHeight(mWidgetDesc->widget_dimensions.size.height + mDesc->contextmenu_itemHeight);
	}

	void ContextMenu::closeMenu()
	{
		// Close all SubMenus
		for(std::vector<Menu*>::iterator it = mSubMenus.begin(); it != mSubMenus.end(); ++it)
			(*it)->closeMenu();

		setVisible(false);
	}

	void ContextMenu::closeSubMenus()
	{
		for(std::vector<Menu*>::iterator it = mSubMenus.begin(); it != mSubMenus.end(); ++it)
		{
			(*it)->closeMenu();
		}
	}

	MenuImageItem* ContextMenu::createImageItem(int index)
	{
		MenuImageItemDesc* d = DescManager::getSingletonPtr()->getDefaultMenuImageItemDesc();
		d->resetToDefault();
		
		MenuImageItem* i = dynamic_cast<MenuImageItem*>(createMenuItem(d,index));

		d->resetToDefault();

		return i;
	}

	MenuImageItem* ContextMenu::createImageItem(const Ogre::String& skin, int index)
	{
		MenuImageItemDesc* d = DescManager::getSingletonPtr()->getDefaultMenuImageItemDesc();
		d->resetToDefault();
		d->widget_skinTypeName = skin;
		
		MenuImageItem* i = dynamic_cast<MenuImageItem*>(createMenuItem(d,index));

		d->resetToDefault();

		return i;
	}

	MenuItem* ContextMenu::createMenuItem(MenuItemDesc* d, int index)
	{
		if(d->widget_name == "")
		{
			d->widget_name = getName() + ".AutoName.MenuItem." + Ogre::StringConverter::toString(mAutoNameCounter);
			++mAutoNameCounter;
		}

		// Make sure new MenuItem will maintain link to owner ContextMenu
		d->contextMenu = this;

		d->widget_horizontalAnchor = ANCHOR_HORIZONTAL_LEFT_RIGHT;

		// Determine position of MenuItem
		d->widget_dimensions.size.width = mClientDimensions.size.width;
		d->widget_dimensions.size.height = mDesc->contextmenu_itemHeight;
		d->index = index;
		
		MenuItem* newMenuItem = dynamic_cast<MenuItem*>(_createWidget(d));
		addChild(newMenuItem);

		return newMenuItem;
	}

	MenuTextItem* ContextMenu::createTextItem(const Ogre::UTFString& text, int index)
	{
		MenuTextItemDesc* d = DescManager::getSingletonPtr()->getDefaultMenuTextItemDesc();
		d->resetToDefault();
		d->textDesc.segments.push_back(TextSegment(text));

		MenuTextItem* i = dynamic_cast<MenuTextItem*>(createMenuItem(d,index));

		d->resetToDefault();

		return i;
	}

	MenuTextItem* ContextMenu::createTextItem(std::vector<TextSegment> segments, int index)
	{
		MenuTextItemDesc* d = DescManager::getSingletonPtr()->getDefaultMenuTextItemDesc();
		d->resetToDefault();
		d->textDesc.segments = segments;

		MenuTextItem* i = dynamic_cast<MenuTextItem*>(createMenuItem(d,index));

		d->resetToDefault();

		return i;
	}

	MenuTextItem* ContextMenu::createTextItem(int index)
	{
		return createTextItem("MenuTextItem",index);
	}

	Menu* ContextMenu::createSubMenu(const Ogre::UTFString& text, int index)
	{
		MenuDesc* d = DescManager::getSingletonPtr()->getDefaultMenuDesc();
		d->resetToDefault();
		d->textDesc.segments.push_back(TextSegment(text));

		Menu* m = dynamic_cast<Menu*>(createMenuItem(d,index));

		d->resetToDefault();

		return m;
	}

	Menu* ContextMenu::createSubMenu(std::vector<TextSegment> segments, int index)
	{
		MenuDesc* d = DescManager::getSingletonPtr()->getDefaultMenuDesc();
		d->resetToDefault();
		d->textDesc.segments = segments;

		Menu* m = dynamic_cast<Menu*>(createMenuItem(d,index));

		d->resetToDefault();

		return m;
	}

	Menu* ContextMenu::createSubMenu(int index)
	{
		return createSubMenu("SubMenu",index);
	}

	void ContextMenu::destroyItem(unsigned int index)
	{
		WidgetFactory* f = FactoryManager::getSingleton().getWidgetFactory();

		bool updateItems = false;
		float yPos = 0;
		int count = 0;
		for(std::list<MenuItem*>::iterator it = mItems.begin(); it != mItems.end(); ++it)
		{
			if(updateItems)
			{
				// Update index
				(*it)->setIndex(count);

				// Update Position
				Point p = (*it)->getPosition();
				p.y = yPos;
				(*it)->setPosition(p);

				// Store position of next item
				yPos += (*it)->getSize().height;
			}

			if(!updateItems && (count == index))
			{
				// Remove Item from list.  All items after this will need to have
				// index and position updated.

				yPos = (*it)->getPosition().y;
				updateItems = true;

				removeChild((*it));

				if(mDesc->sheet != NULL)
					mDesc->sheet->mFreeList.push_back((*it));
				else
					Root::getSingleton().mGUIManagers.begin()->second->mFreeList.push_back((*it));
				mItems.erase(it);

				--count;
			}

			++count;
		}

		redraw();
	}

	void ContextMenu::destroyItem(MenuItem* i)
	{
		unsigned int count = 0;
		for(std::list<MenuItem*>::iterator it = mItems.begin(); it != mItems.end(); ++it)
		{
			if((*it) == i)
			{
				destroyItem(count);
				return;
			}

			++count;
		}
	}

	MenuItem* ContextMenu::getItem(unsigned int index)
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

	float ContextMenu::getItemHeight()
	{
		return mDesc->contextmenu_itemHeight;
	}

	int ContextMenu::getNumberOfItems()
	{
		return static_cast<int>(mItems.size());
	}

	bool ContextMenu::hasSubMenu(Menu* m)
	{
		// recurse submenus to see if menu is a submenu
		for(std::vector<Menu*>::iterator it = mSubMenus.begin(); it != mSubMenus.end(); ++it)
		{
			if( (*it)->hasSubMenu(m) )
				return true;
		}

		return false;
	}

	void ContextMenu::hide()
	{
		closeMenu();
	}

	void ContextMenu::onClientSizeChanged(const EventArgs& args)
	{
		for(std::vector<Widget*>::iterator it = mChildren.begin(); it != mChildren.end(); ++it)
		{
			(*it)->setWidth(mClientDimensions.size.width);
		}
	}

	void ContextMenu::onLoseFocus(const EventArgs& args)
	{
		if(findWidget(mDesc->sheet->getWindowInFocus()->getName()) == NULL)
			hide();
	}

	void ContextMenu::onPositionChanged(const EventArgs& args)
	{
		closeSubMenus();
	}

	void ContextMenu::onVisibleChanged(const EventArgs& args)
	{
		if(mDesc->widget_visible)
			mDesc->sheet->focusWindow(this);
		else
			// Make sure SubMenus are closed. (ie pressing 'X' button while menus are visible)
			closeSubMenus();
	}

	void ContextMenu::openMenu(const Point& position)
	{
		// Close all SubMenus
		for(std::vector<Menu*>::iterator it = mSubMenus.begin(); it != mSubMenus.end(); ++it)
			(*it)->closeMenu();

		mWidgetDesc->widget_dimensions.position = position;

		if(position.y < 0)
			mWidgetDesc->widget_dimensions.position.y = 0;
		else if((position.y + mWidgetDesc->widget_dimensions.size.height) > mWidgetDesc->sheet->getHeight())
			mWidgetDesc->widget_dimensions.position.y = mWidgetDesc->sheet->getHeight() - mWidgetDesc->widget_dimensions.size.height;

		if(position.x < 0)
			mWidgetDesc->widget_dimensions.position.x = 0;
		else if((position.x + mWidgetDesc->widget_dimensions.size.width) > mWidgetDesc->sheet->getWidth())
			mWidgetDesc->widget_dimensions.position.x = mWidgetDesc->sheet->getWidth() - mWidgetDesc->widget_dimensions.size.width;

		setVisible(true);

		// If this menu is already visible, setVisible won't toggle redrawing
		redraw();
	}

	void ContextMenu::openSubMenu(Menu* m)
	{
		openMenu(mWidgetDesc->widget_dimensions.position);

		for(std::vector<Menu*>::iterator it = mSubMenus.begin(); it != mSubMenus.end(); ++it)
		{
			if((*it)->hasSubMenu(m))
				(*it)->openSubMenu(m);
			else
				(*it)->closeMenu();
		}
	}

	void ContextMenu::setItemHeight(float height)
	{
		mDesc->contextmenu_itemHeight = height;

		_updateItemPositions();

		setHeight(mItems.back()->getPosition().y + mItems.back()->getHeight());
	}

	void ContextMenu::show(const Point& position)
	{
		openMenu(position);
	}
}
