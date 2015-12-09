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
