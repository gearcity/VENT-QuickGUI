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

#include "QuickGUITabPage.h"
#include "QuickGUISkinDefinitionManager.h"
#include "QuickGUITabControl.h"
#include "QuickGUIManager.h"
#include "QuickGUIScriptDefinition.h"
#include "QuickGUIFactoryManager.h"
#include "QuickGUISkinDefinition.h"
#include "QuickGUITab.h"
#include "QuickGUIDescManager.h"
#include "QuickGUIWidgetFactory.h"

#include "OgreFont.h"

namespace QuickGUI
{
	const Ogre::String TabPage::TAB = "tab";
	const Ogre::String TabPage::TAB_DISABLED = "tab_disabled";
	const Ogre::String TabPage::TAB_OVER = "tab_over";
	const Ogre::String TabPage::TAB_SELECTED = "tab_selected";
	const Ogre::String TabPage::PAGE = "page";

	void TabPage::registerSkinDefinition()
	{
		SkinDefinition* d = OGRE_NEW_T(SkinDefinition,Ogre::MEMCATEGORY_GENERAL)("TabPage");
		d->defineSkinReference(TAB,"Tab");
		d->defineSkinReference(TAB_OVER,"Tab");
		d->defineSkinReference(TAB_SELECTED,"Tab");
		d->defineSkinReference(PAGE,"Panel");
		d->definitionComplete();

		SkinDefinitionManager::getSingleton().registerSkinDefinition("TabPage",d);
	}

	TabPage::TabPage(const Ogre::String& name) :
		Panel(name),
		mTab(NULL),
		mPage(NULL),
		mSelected(false),
		mTabOverlap(3)
	{
	}

	TabPage::~TabPage()
	{
		// Clear children, they really belong to the child Panel, ie Page
		mChildren.clear();
	}

	void TabPage::_initialize(WidgetDesc* d)
	{
		TabPageDesc* tpd = dynamic_cast<TabPageDesc*>(d);
		tpd->containerwidget_supportScrollBars = false;
		tpd->widget_horizontalAnchor = ANCHOR_HORIZONTAL_LEFT_RIGHT;
		tpd->widget_verticalAnchor = ANCHOR_VERTICAL_TOP_BOTTOM;

		Panel::_initialize(d);

		mDesc = dynamic_cast<TabPageDesc*>(mWidgetDesc);
		mDesc->tabpage_index = tpd->tabpage_index;
		mDesc->tabpage_tabFrontWidth = tpd->tabpage_tabFrontWidth;
		mDesc->tabpage_tabImageName = tpd->tabpage_tabImageName;

		// Create Tab
		TabDesc* td = DescManager::getSingletonPtr()->getDefaultTabDesc();
		td->resetToDefault();
		td->widget_name = getName() + ".Tab";
		td->tab_frontWidth = tpd->tabpage_tabFrontWidth;
		td->tab_imageName = tpd->tabpage_tabImageName;
		td->textDesc = tpd->textDesc;
		mTab = dynamic_cast<Tab*>(_createWidget(td));
		mTab->addWidgetEventHandler(WIDGET_EVENT_MOUSE_BUTTON_DOWN,&TabPage::onMouseButtonDownOnTab,this);
		mTab->addWidgetEventHandler(WIDGET_EVENT_MOUSE_ENTER,&TabPage::onMouseEnterTab,this);
		mTab->addWidgetEventHandler(WIDGET_EVENT_MOUSE_LEAVE,&TabPage::onMouseLeaveTab,this);
		addComponent(TAB,mTab);

		// Create Page
		PanelDesc* pd = DescManager::getSingletonPtr()->getDefaultPanelDesc();
		pd->resetToDefault();
		pd->widget_name = getName() + ".Page";
		pd->widget_dimensions.position = Point(0,mTab->getHeight());
		pd->widget_dimensions.size = Size(mDesc->widget_dimensions.size.width,mDesc->widget_dimensions.size.height - mTab->getHeight());
		pd->widget_horizontalAnchor = ANCHOR_HORIZONTAL_LEFT_RIGHT;
		pd->widget_verticalAnchor = ANCHOR_VERTICAL_TOP_BOTTOM;
		mPage = dynamic_cast<Panel*>(_createWidget(pd));
		addComponent(PAGE,mPage);

		setSkinType(d->widget_skinTypeName);
		// This API is called prior to creation of Tab/Page widgets, and we need to set the properties of these
		// widgets.
		setTransparencyPicking(d->widget_transparencyPicking);
	}

	void TabPage::addChild(Widget* w)
	{
		mPage->addChild(w);

		mChildren.push_back(w);
	}

	void TabPage::addWidgetEventHandler(WidgetEvent EVENT, EventHandlerSlot* function)
	{
		mPage->addWidgetEventHandler(EVENT,function);
	}

	void TabPage::deselect()
	{
		if(mSelected)
		{
			mSelected = false;
			mTab->setSkinType(mSkinType->getSkinReference(TAB)->typeName);
			mPage->setVisible(false);
			redraw();
		}
	}

	void TabPage::draw()
	{
		// check visibility
		if( !mWidgetDesc->widget_visible )
			return;

		Brush* brush = Brush::getSingletonPtr();

		// store clip region
		Rect prevClipRegion = brush->getClipRegion();

		// set clip region to dimensions
		Rect widgetClipRegion = prevClipRegion;
		widgetClipRegion.getIntersection(Rect(mTexturePosition,mWidgetDesc->widget_dimensions.size));

		// set clip region to dimensions
		brush->setClipRegion(widgetClipRegion);

		// Set color before drawing operations
		brush->setColor(mWidgetDesc->widget_baseColor);

		// Set opacity before drawing operations
		brush->setOpacity(getAbsoluteOpacity());

		// check and clip region - do not draw if we are outside clipping region
		if (widgetClipRegion != Rect::ZERO )
		{
			// draw self
			onDraw();
		}

		// set clip region to client dimensions

		Rect clientClipRegion = prevClipRegion;
		clientClipRegion.getIntersection(Rect(mTexturePosition,mClientDimensions.size));
		brush->setClipRegion(clientClipRegion);

		// draw components
		for(std::map<Ogre::String,Widget*>::iterator it = mComponents.begin(); it != mComponents.end(); ++it)
			(*it).second->draw();

		// restore clip region
		brush->setClipRegion(prevClipRegion);
	}

	Widget* TabPage::findWidgetAtPoint(const Point& p, unsigned int queryFilter, bool ignoreDisabled)
	{
		// If we are not visible, return NULL
		if(!mWidgetDesc->widget_visible)
			return NULL;

		if((mWidgetDesc->widget_queryFlags & queryFilter) == 0)
			return NULL;

		// If we ignore disabled and this widget is enabled, return NULL
		if(ignoreDisabled && !mWidgetDesc->widget_enabled)
			return NULL;

		Widget* w = NULL;
		
		// Test to see if the position is over the tab
		w = mTab->findWidgetAtPoint(p,ignoreDisabled);
		if(w != NULL)
			return w;

		// Test to see if the position is over the Page (Panel) or its children
		w = mPage->findWidgetAtPoint(p,ignoreDisabled);
		if(w != NULL)
			return w;
		
		return NULL;
	}

	BorderSide TabPage::getBorderSide(Point p)
	{
		return BORDER_NONE;
	}

	Ogre::String TabPage::getClass()
	{
		return "TabPage";
	}

	int TabPage::getIndex()
	{
		return mDesc->tabpage_index;
	}

	Size TabPage::getPageClientArea()
	{
		return mPage->getClientDimensions().size;
	}

	bool TabPage::isPositionManaged()
	{
		return true;
	}

	bool TabPage::isSizeManaged()
	{
		return true;
	}

	bool TabPage::isSelected()
	{
		return mSelected;
	}

	void TabPage::onDraw()
	{
		// Nothing drawn for TabPage, its just a container for Tab and Panel (Page)
	}

	void TabPage::onMouseButtonDownOnTab(const EventArgs& args)
	{
		const MouseEventArgs& mea = dynamic_cast<const MouseEventArgs&>(args);

		if(mea.button == MB_Left)
		{
			if(mParentWidget != NULL)
			{
				dynamic_cast<TabControl*>(mParentWidget)->selectTabPage(this);
			}
		}
	}

	void TabPage::onMouseEnterTab(const EventArgs& args)
	{
		const MouseEventArgs& mea = dynamic_cast<const MouseEventArgs&>(args);

		TabControl* tc = dynamic_cast<TabControl*>(mParentWidget);

		if(tc->getTabReordering() && (mea.buttonMask & (1 << MB_Left)))
		{
			Widget* w = mDesc->guiManager->getLastClickedWidget();
			if((w != NULL) && (w->getClass() == "Tab") && (w != mTab))
			{
				TabPage* p = dynamic_cast<TabPage*>(w->getParentWidget());
				tc->reorderTabPage(p,getIndex());
				return;
			}
		}

		if(!mSelected)
		{
			mTab->setSkinType(mSkinType->getSkinReference(TAB_OVER)->typeName);
			redraw();
		}
	}

	void TabPage::onMouseLeaveTab(const EventArgs& args)
	{
		if(!mSelected)
		{
			mTab->setSkinType(mSkinType->getSkinReference(TAB)->typeName);
			redraw();
		}
	}

	bool TabPage::overBorderSide(Point p)
	{
		return false;
	}

	void TabPage::removeChild(Widget* w)
	{
		mPage->removeChild(w);

		std::list<Widget*>::iterator it = std::find(mChildren.begin(),mChildren.end(),w);
		if(it == mChildren.end())
			throw Exception(Exception::ERR_INVALID_CHILD,"Widget \"" + w->getName() + "\" is not a child of widget \"" + getName() + "\"","TabPage::removeChild");

		mChildren.erase(it);
	}

	void TabPage::select()
	{
		if(!mSelected)
		{
			mSelected = true;
			mPage->setVisible(true);
			mTab->setSkinType(mSkinType->getSkinReference(TAB_SELECTED)->typeName);
			redraw();
		}
	}

	void TabPage::serialize(SerialBase* b)
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

		// If we are reading, we need to create and populate widgets
		if(b->isSerialReader())
		{
			// Serial Read into our TextDesc object
			dynamic_cast<TabPageDesc*>(mWidgetDesc)->textDesc.serialize(b);

			if(b->begin("Child","Widgets"))
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
				b->end();
			}
		}
		// If we are writing, we need to serialize children
		else
		{
			// Serialize Tab's text.  When serial reading, we read into TabPage's text object
			mTab->mDesc->textDesc.serialize(b);

			// Serialize Page's children.  They will be properly added on serial read since TabPage::addChild really calls
			// the pages addChild function.
			b->begin("Child","Widgets");
			for(std::list<Widget*>::iterator it = mPage->mChildren.begin(); it != mPage->mChildren.end(); ++it)
			{
				(*it)->serialize(b);
			}
			b->end();
		}

		b->end();
	}

	void TabPage::setEnabled(bool enabled)
	{
		Panel::setEnabled(enabled);

		// disable to tab so users cannot select tab via clicking.
		if(mTab != NULL)
		{
			mTab->setEnabled(enabled);

			if(enabled)
				mTab->setSkinType(mSkinType->getSkinReference(TAB)->typeName);
			else
				mTab->setSkinType(mSkinType->getSkinReference(TAB_DISABLED)->typeName);
		}
	}

	void TabPage::setIndex(unsigned int index)
	{
		mDesc->tabpage_index = index;
	}

	void TabPage::setParent(Widget* parent)
	{
		Panel::setParent(parent);

		if(parent != NULL)
			dynamic_cast<TabControl*>(parent)->selectTabPage(this);
	}

	void TabPage::setSkinType(const Ogre::String type)
	{
		Widget::setSkinType(type);
		
		if(mPage != NULL)
		{
			mPage->setSkinType(mSkinType->getSkinReference(PAGE)->typeName);
		}
	}

	void TabPage::setTabFont(const Ogre::String& fontName)
	{
		mTab->setFont(fontName);
	}

	void TabPage::setTabFont(const Ogre::String& fontName, unsigned int index)
	{
		mTab->setFont(fontName,index);
	}

	void TabPage::setTabFont(const Ogre::String& fontName, unsigned int startIndex, unsigned int endIndex)
	{
		mTab->setFont(fontName,startIndex,endIndex);
	}

	void TabPage::setTabFont(const Ogre::String& fontName, Ogre::UTFString::code_point c, bool allOccurrences)
	{
		mTab->setFont(fontName,c,allOccurrences);
	}

	void TabPage::setTabFont(const Ogre::String& fontName, Ogre::UTFString s, bool allOccurrences)
	{
		mTab->setFont(fontName,s,allOccurrences);
	}

	void TabPage::setTabHeight(float height)
	{
		if((mTab != NULL) && (mPage != NULL))
		{
			mTab->setHeight(height);

			mPage->setPosition(Point(0,mTab->getHeight() - mTabOverlap));
			mPage->setHeight(mDesc->widget_dimensions.size.height - mTab->getHeight() + mTabOverlap);
		}
	}

	void TabPage::setTabText(Ogre::UTFString s, Ogre::Font* fp, const ColourValue& cv)
	{
		mTab->setText(s,fp,cv);
	}

	void TabPage::setTabText(Ogre::UTFString s, const Ogre::String& fontName, const ColourValue& cv)
	{
		mTab->setText(s,fontName,cv);
	}

	void TabPage::setTabText(Ogre::UTFString s)
	{
		mTab->setText(s);
	}

	void TabPage::setTransparencyPicking(bool transparencyPicking)
	{
		Panel::setTransparencyPicking(transparencyPicking);

		if(mTab != NULL)
			mTab->setTransparencyPicking(transparencyPicking);
		if(mPage != NULL)
			mPage->setTransparencyPicking(transparencyPicking);
	}

	void TabPage::updateClientDimensions()
	{
		// This is a modified version of ContainerWidget::updateClientDimensions.
		// Basically the TabPage class stores reference to Panel's children in its own children list,
		// and we need to make sure children are updated by their actual parent, which is the Panel, not the TabPage.
		// We also shouldn't worry about the Tab, and just apply anchoring to the Panel.

		Size previousSize = mClientDimensions.size;

		mClientDimensions.position = Point::ZERO;
		mClientDimensions.size = mWidgetDesc->widget_dimensions.size;

		// Get difference.  A larger size indicates a positive difference
		Size difference = mClientDimensions.size - previousSize;

		// grow or shrink the widget according to new dimensions.  The Panel is assumed to have anchors
		// ANCHOR_HORIZONTAL_LEFT_RIGHT and ANCHOR_VERTICAL_TOP_BOTTOM
		if(mPage != NULL)
		{
			Size panelSize = mPage->getSize();
			panelSize = panelSize + difference;
			mPage->setSize(panelSize);
		}

		WidgetEventArgs args(this);
		fireWidgetEvent(WIDGET_EVENT_CLIENTSIZE_CHANGED,args);
	}

	void TabPage::updateSkinElement()
	{
		mSkinElement = NULL;
	}
}
