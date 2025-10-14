#include "QuickGUIMenuPanel.h"
#include "QuickGUIListItem.h"
#include "QuickGUISkinDefinitionManager.h"
#include "QuickGUIManager.h"
#include "QuickGUIContextMenu.h"
#include "QuickGUISheet.h"
#include "QuickGUISkinDefinition.h"
#include "QuickGUIComboBox.h"
#include "QuickGUIToolBar.h"
#include "QuickGUIMenu.h"

namespace QuickGUI
{
	const Ogre::String MenuPanel::BACKGROUND = "background";

	void MenuPanel::registerSkinDefinition()
	{
		SkinDefinition* d = OGRE_NEW_T(SkinDefinition,Ogre::MEMCATEGORY_GENERAL)("MenuPanel");
		d->defineSkinElement(BACKGROUND);
		d->defineComponent(HSCROLLBAR);
		d->defineComponent(VSCROLLBAR);
		d->definitionComplete();

		SkinDefinitionManager::getSingleton().registerSkinDefinition("MenuPanel",d);
	}

	MenuPanelDesc::MenuPanelDesc() :
		WindowDesc()
	{
		resetToDefault();
	}

	void MenuPanelDesc::resetToDefault()
	{
		WindowDesc::resetToDefault();

		menupanel_owner = NULL;
		menupanel_maxHeight = 0;

		containerwidget_supportScrollBars = true;

		window_titleBar = false;

		WindowDesc::window_titleBar = false;

		widget_resizeFromBottom = false;
		widget_resizeFromLeft = false;
		widget_resizeFromRight = false;
		widget_resizeFromTop = false;
		widget_serialize = false;
	}

	MenuPanel::MenuPanel(const Ogre::String& name) :
		Window(name),
		mDesc(NULL)
	{
		addWindowEventHandler(WINDOW_EVENT_FOCUS_LOST,&MenuPanel::onLoseFocus,this);
		addWidgetEventHandler(WIDGET_EVENT_VISIBLE_CHANGED,&MenuPanel::onVisibleChanged,this);
	}

	MenuPanel::~MenuPanel()
	{
	}

	void MenuPanel::_adjustHeight()
	{
		// Update MenuPanel height - for now, just expand to fit all items
		float totalItemHeight = 0;
		for(std::vector<Widget*>::iterator it = mChildren.begin(); it != mChildren.end(); ++it)
		{
			if((*it)->getVisible())
			{
				float y = (*it)->getPosition().y + (*it)->getHeight();
				if(y > totalItemHeight)
					totalItemHeight = y;
			}
		}

		// Adjust Height
		SkinElement* background = mSkinType->getSkinElement(BACKGROUND);
		float desiredHeight = totalItemHeight + (background->getBorderThickness(BORDER_TOP) + background->getBorderThickness(BORDER_BOTTOM));

		if(mDesc->menupanel_maxHeight > 0)
		{
			if(desiredHeight < mDesc->menupanel_maxHeight)
				setHeight(desiredHeight);
			else
				setHeight(mDesc->menupanel_maxHeight);
		}
		else
			setHeight(desiredHeight);
	}

	void MenuPanel::_initialize(WidgetDesc* d)
	{
		d->widget_resizeFromBottom = false;
		d->widget_resizeFromLeft = false;
		d->widget_resizeFromRight = false;
		d->widget_resizeFromTop = false;
		d->widget_serialize = false;

		Window::_initialize(d);

		mDesc = dynamic_cast<MenuPanelDesc*>(mWidgetDesc);

		MenuPanelDesc* lpd = dynamic_cast<MenuPanelDesc*>(d);

		mDesc->menupanel_owner = lpd->menupanel_owner;
		mDesc->menupanel_maxHeight = lpd->menupanel_maxHeight;

		setSkinType(d->widget_skinTypeName);
	}

	void MenuPanel::addWidget(Widget* w)
	{
		addChild(w);

		_adjustHeight();
	}

	float MenuPanel::getAbsoluteOpacity()
	{
		if(!mWidgetDesc->widget_inheritOpacity || (mDesc->menupanel_owner == NULL))
			return mWidgetDesc->widget_relativeOpacity;

		return mDesc->menupanel_owner->getAbsoluteOpacity() * mWidgetDesc->widget_relativeOpacity;
	}

	Ogre::String MenuPanel::getClass()
	{
		return "MenuPanel";
	}

	Widget* MenuPanel::getOwner()
	{
		return mDesc->menupanel_owner;
	}

	bool MenuPanel::isChildOf(Widget* w)
	{
		if(w == NULL)
			return false;

		Widget* parent = getOwner();

		if(parent == NULL)
			return false;

		if(parent == w)
			return true;

		return (parent->isChildOf(w));
	}

	void MenuPanel::onDraw()
	{
		Brush* brush = Brush::getSingletonPtr();

		brush->setFilterMode(mDesc->widget_brushFilterMode);

		brush->drawSkinElement(Rect(Point::ZERO,mWidgetDesc->widget_dimensions.size),mSkinElement);
	}

	void MenuPanel::onLoseFocus(const EventArgs& args)
	{
		if(mDesc->menupanel_owner != NULL)
		{
			Ogre::String ownerClass = mDesc->menupanel_owner->getClass();
			if(ownerClass == "Menu")
			{
				ToolBar* tb = dynamic_cast<Menu*>(mDesc->menupanel_owner)->getToolBar();
				if(tb != NULL)
				{
					if(mDesc->guiManager->getLastClickedWidget() == NULL)
						tb->closeMenus();
					else if(!mDesc->guiManager->getLastClickedWidget()->isComponentOf(tb) && 
							!mDesc->guiManager->getLastClickedWidget()->isChildOf(tb))
						tb->closeMenus();
				}
				else
				{
					ContextMenu* cm = dynamic_cast<Menu*>(mDesc->menupanel_owner)->getContextMenu();
					if((cm != NULL) && (cm->findWidget(mDesc->sheet->getWindowInFocus()->getName()) == NULL))
						cm->hide();
				}
			}
			else if(ownerClass == "ComboBox")
			{
				ComboBox* cb = dynamic_cast<ComboBox*>(mDesc->menupanel_owner);

				// If we click a widget other than the combobox, hide the list.
				// If we did click the combobox, the combobox will hide the list automatically.
				//if(mDesc->guiManager->getLastClickedWidget() == NULL)
					cb->hideDropDownList();
				//else if(!mDesc->guiManager->getLastClickedWidget()->isComponentOf(cb))
				//	cb->hideDropDownList();
			}
		}
	}

	void MenuPanel::onVisibleChanged(const EventArgs& args)
	{
		if(mWidgetDesc->widget_visible)
		{
			mWidgetDesc->sheet->focusWindow(this);
		}
	}

	void MenuPanel::removeWidget(Widget* w)
	{			
		removeChild(w);

		_adjustHeight();
	}

	void MenuPanel::removeWidgets()
	{
		removeChildren();

		_adjustHeight();
	}

	void MenuPanel::setMaxHeight(float height)
	{
		mDesc->menupanel_maxHeight = height;

		_adjustHeight();
	}

	void MenuPanel::updateClientDimensions()
	{
		Window::updateClientDimensions();

		for(std::vector<Widget*>::iterator it = mChildren.begin(); it != mChildren.end(); ++it)
		{
			(*it)->setWidth(mClientDimensions.size.width);
		}
	}

	void MenuPanel::destroyWidgets()
	{
		Window::destroyChildren();

		_adjustHeight();
	}
}
