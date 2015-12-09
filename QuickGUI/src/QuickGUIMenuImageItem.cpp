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

#include "QuickGUIMenuImageItem.h"
#include "QuickGUIToolBar.h"
#include "QuickGUIContextMenu.h"
#include "QuickGUISkinDefinitionManager.h"
#include "QuickGUISkinDefinition.h"
#include "QuickGUIMenu.h"

namespace QuickGUI
{
	const Ogre::String MenuImageItem::DEFAULT = "default";
	const Ogre::String MenuImageItem::DOWN = "down";
	const Ogre::String MenuImageItem::OVER = "over";

	void MenuImageItem::registerSkinDefinition()
	{
		SkinDefinition* d = OGRE_NEW_T(SkinDefinition,Ogre::MEMCATEGORY_GENERAL)("MenuImageItem");
		d->defineSkinElement(DEFAULT);
		d->defineSkinElement(OVER);
		d->defineSkinElement(DOWN);
		d->definitionComplete();

		SkinDefinitionManager::getSingleton().registerSkinDefinition("MenuImageItem",d);
	}

	MenuImageItemDesc::MenuImageItemDesc() :
		MenuItemDesc()
	{
		resetToDefault();
	}

	void MenuImageItemDesc::resetToDefault()
	{
		MenuItemDesc::resetToDefault();
	}

	void MenuImageItemDesc::serialize(SerialBase* b)
	{
		MenuItemDesc::serialize(b);
	}

	MenuImageItem::MenuImageItem(const Ogre::String& name) :
		MenuItem(name)
	{
	}

	MenuImageItem::~MenuImageItem()
	{
	}

	void MenuImageItem::_initialize(WidgetDesc* d)
	{
		MenuItem::_initialize(d);

		mDesc = dynamic_cast<MenuImageItemDesc*>(mWidgetDesc);

		MenuImageItemDesc* mld = dynamic_cast<MenuImageItemDesc*>(d);

		setSkinType(d->widget_skinTypeName);

		// Set the width of the MenuImageItem to the width of the Skin texture by default.
		setWidth(mSkinElement->getWidth());
	}

	Ogre::String MenuImageItem::getClass()
	{
		return "MenuImageItem";
	}

	void MenuImageItem::onDraw()
	{
		Brush* brush = Brush::getSingletonPtr();

		brush->setFilterMode(mDesc->widget_brushFilterMode);

		brush->drawSkinElement(Rect(mTexturePosition,mWidgetDesc->widget_dimensions.size),mSkinElement);
	}

	void MenuImageItem::onMouseButtonUp(const EventArgs& args)
	{
		const MouseEventArgs& mea = dynamic_cast<const MouseEventArgs&>(args);

		if(mea.button == MB_Left)
		{
			MenuItemDesc* menuItemDesc = dynamic_cast<MenuItemDesc*>(mWidgetDesc);
			
			if(menuItemDesc->toolBar != NULL)
				menuItemDesc->toolBar->closeMenus();
			else if(menuItemDesc->contextMenu != NULL)
				menuItemDesc->contextMenu->hide();
		}
	}

	void MenuImageItem::onMouseEnter(const EventArgs& args)
	{
		if(mDesc->menu != NULL)
			mDesc->menu->closeSubMenus();
		else if(mDesc->toolBar != NULL)
			mDesc->toolBar->closeMenus();
		else if(mDesc->contextMenu != NULL)
			mDesc->contextMenu->closeSubMenus();

		mSkinElement = mSkinType->getSkinElement(OVER);

		redraw();
	}

	void MenuImageItem::onMouseLeave(const EventArgs& args)
	{
		mSkinElement = mSkinType->getSkinElement(DEFAULT);

		redraw();
	}

	void MenuImageItem::updateSkinElement()
	{
		mSkinElement = NULL;
		if(mSkinType != NULL)
			mSkinElement = mSkinType->getSkinElement(DEFAULT);
	}
}
