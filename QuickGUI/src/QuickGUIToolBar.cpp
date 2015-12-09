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

#include "QuickGUIToolBar.h"
#include "QuickGUIManager.h"
#include "QuickGUISkinDefinitionManager.h"
#include "QuickGUIFactoryManager.h"
#include "QuickGUISkinDefinition.h"
#include "QuickGUIMenu.h"
#include "QuickGUIDescManager.h"

namespace QuickGUI
{
	const Ogre::String ToolBar::BACKGROUND = "background";

	void ToolBar::registerSkinDefinition()
	{
		SkinDefinition* d = OGRE_NEW_T(SkinDefinition,Ogre::MEMCATEGORY_GENERAL)("ToolBar");
		d->defineSkinElement(BACKGROUND);
		d->definitionComplete();

		SkinDefinitionManager::getSingleton().registerSkinDefinition("ToolBar",d);
	}

	ToolBarDesc::ToolBarDesc() :
		ContainerWidgetDesc()
	{
		resetToDefault();
	}

	void ToolBarDesc::resetToDefault()
	{
		ContainerWidgetDesc::resetToDefault();

		widget_dimensions.size = Size(125,25);

		containerwidget_supportScrollBars = false;
		toolbar_itemLayout = TOOLBAR_ITEM_LAYOUT_NEGATIVE_TO_POSITIVE;
	}

	void ToolBarDesc::serialize(SerialBase* b)
	{
		ContainerWidgetDesc::serialize(b);

		// Retrieve default values to supply to the serial reader/writer.
		// The reader uses the default value if the given property does not exist.
		// The writer does not write out the given property if it has the same value as the default value.
		ToolBarDesc* defaultValues = DescManager::getSingleton().createDesc<ToolBarDesc>(getClass(),"temp");
		defaultValues->resetToDefault();

		b->IO("ItemLayout", &toolbar_itemLayout, defaultValues->toolbar_itemLayout);

		DescManager::getSingleton().destroyDesc(defaultValues);
	}

	ToolBar::ToolBar(const Ogre::String& name) :
		ContainerWidget(name),
		mCurrentState(TOOLBAR_STATE_DEFAULT),
		mMenuOpened(false)
	{
	}

	ToolBar::~ToolBar()
	{
	}

	void ToolBar::_initialize(WidgetDesc* d)
	{
		ContainerWidget::_initialize(d);

		mDesc = dynamic_cast<ToolBarDesc*>(mWidgetDesc);

		ToolBarDesc* tbd = dynamic_cast<ToolBarDesc*>(d);

		mDesc->toolbar_itemLayout = tbd->toolbar_itemLayout;

		if(mWidgetDesc->widget_dimensions.size.width > mWidgetDesc->widget_dimensions.size.height)
			mOrientation = TOOLBAR_ORIENTATION_HORIZONTAL;
		else
			mOrientation = TOOLBAR_ORIENTATION_VERTICAL;

		setSkinType(d->widget_skinTypeName);
	}

	void ToolBar::_updateItemPositions()
	{
		Point p;
		int count = 0;
		for(std::list<ToolBarItem*>::iterator it = mItems.begin(); it != mItems.end(); ++it)
		{
			(*it)->setPosition(p);
			(*it)->setIndex(count);

			if((*it)->getVisible())
			{
				if(mOrientation == TOOLBAR_ORIENTATION_HORIZONTAL)
					p.x += (*it)->getWidth();
				else
					p.y += (*it)->getHeight();
			}

			++count;
		}
	}

	void ToolBar::addChild(Widget* w)
	{
		if(!w->isToolBarItem())
			throw Exception(Exception::ERR_INVALID_CHILD,"Cannot add a non-ToolBarItem to a ToolBar!","ToolBar::addChild");

		ContainerWidget::addChild(w);

		if(mOrientation == TOOLBAR_ORIENTATION_HORIZONTAL)
		{
			w->setHeight(mClientDimensions.size.height);
			w->setVerticalAnchor(ANCHOR_VERTICAL_TOP_BOTTOM);
		}
		else
		{
			w->setWidth(mClientDimensions.size.width);
			w->setHorizontalAnchor(ANCHOR_HORIZONTAL_LEFT_RIGHT);
		}

		// Update link to parent ToolBar
		ToolBarItem* i = dynamic_cast<ToolBarItem*>(w);
		
		i->notifyToolBarParent(this);

		int itemIndex = i->getIndex();

		// Adding Item to End of List
		if((itemIndex < 0) || (itemIndex >= static_cast<int>(mItems.size())))
		{
			mItems.push_back(i);
		}
		// Inserting Item into the List
		else
		{
			int count = 0;
			for(std::list<ToolBarItem*>::iterator it = mItems.begin(); it != mItems.end(); ++it)
			{
				if(count == itemIndex)
				{
					mItems.insert(it,i);
					break;
				}

				++count;
			}
		}

		if(w->getClass() == "Menu")
		{
			Menu* m = dynamic_cast<Menu*>(w);
			// For menu's that are directly connected to a ToolBar, we set their owner menu reference to themselves.
			m->notifyMenuParent(m);

			mMenus.push_back(m);
		}

		_updateItemPositions();
	}

	void ToolBar::closeMenus()
	{
		for(std::vector<Menu*>::iterator it = mMenus.begin(); it != mMenus.end(); ++it)
			(*it)->closeMenu();

		mMenuOpened = false;
	}

	Menu* ToolBar::createMenu(const Ogre::UTFString& text, int index)
	{
		MenuDesc* d = DescManager::getSingletonPtr()->getDefaultMenuDesc();
		d->resetToDefault();
		d->textDesc.segments.push_back(TextSegment(text));

		Menu* m = dynamic_cast<Menu*>(createToolBarItem(d,index));

		d->resetToDefault();

		return m;
	}

	Menu* ToolBar::createMenu(std::vector<TextSegment> segments, int index)
	{
		MenuDesc* d = DescManager::getSingletonPtr()->getDefaultMenuDesc();
		d->resetToDefault();
		d->textDesc.segments = segments;

		Menu* m = dynamic_cast<Menu*>(createToolBarItem(d,index));

		d->resetToDefault();

		return m;
	}

	Menu* ToolBar::createMenu(int index)
	{
		return createMenu("Menu",index);
	}

	ToolBarItem* ToolBar::createToolBarItem(ToolBarItemDesc* d, int index)
	{
		d->toolBar = this;
		d->index = index;

		ToolBarItem* newItem = dynamic_cast<ToolBarItem*>(_createWidget(d));

		addChild(newItem);

		return newItem;
	}

	void ToolBar::destroyItem(ToolBarItem* i)
	{
		throw Exception(0,"Not Implemented","ToolBar::destroyItem");
	}

	Ogre::String ToolBar::getClass()
	{
		return "ToolBar";
	}

	int ToolBar::getNumberOfItems()
	{
		return static_cast<int>(mItems.size());
	}

	ToolBarOrientation ToolBar::getOrientation()
	{
		return mOrientation;
	}

	bool ToolBar::isMenuOpened()
	{
		return mMenuOpened;
	}

	void ToolBar::onDraw()
	{
		Brush* brush = Brush::getSingletonPtr();

		brush->setFilterMode(mDesc->widget_brushFilterMode);
			
		brush->drawSkinElement(Rect(mTexturePosition,mWidgetDesc->widget_dimensions.size),mSkinElement);
	}

	void ToolBar::openMenu(Menu* m)
	{
		bool found = false;
		for(std::vector<Menu*>::iterator it = mMenus.begin(); it != mMenus.end(); ++it)
		{
			if((*it)->hasSubMenu(m))
			{
				(*it)->openMenu();	
				(*it)->openSubMenu(m);
				found = true;
				mMenuOpened = true;
			}
			else
				(*it)->closeMenu();
		}

		if(!found)
			throw Exception(Exception::ERR_INVALID_CHILD,"Given Menu is not a Child of this ToolBar!","ToolBar::openMenu");
	}

	void ToolBar::updateTexturePosition()
	{
		ContainerWidget::updateTexturePosition();

		closeMenus();
	}
}
