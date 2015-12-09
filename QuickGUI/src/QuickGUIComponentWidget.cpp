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

#include "QuickGUIComponentWidget.h"
#include "QuickGUIBrush.h"
#include "QuickGUISerialWriter.h"
#include "QuickGUIWindow.h"
#include "QuickGUISheet.h"
#include "QuickGUIFactoryManager.h"
#include "QuickGUIWidgetFactory.h"
#include "QuickGUIDescManager.h"

namespace QuickGUI
{
	ComponentWidgetDesc::ComponentWidgetDesc() :
		WidgetDesc()
	{
		resetToDefault();
	}

	void ComponentWidgetDesc::resetToDefault()
	{
		WidgetDesc::resetToDefault();

		componentwidget_clipComponentsToDimensions = true;
	}

	void ComponentWidgetDesc::serialize(SerialBase* b)
	{
		WidgetDesc::serialize(b);

		// Retrieve default values to supply to the serial reader/writer.
		// The reader uses the default value if the given property does not exist.
		// The writer does not write out the given property if it has the same value as the default value.
		ComponentWidgetDesc* defaultValues = DescManager::getSingleton().createDesc<ComponentWidgetDesc>(getClass(),"temp");
		defaultValues->resetToDefault();

		b->IO("ClipComponentsToDimensions",	&componentwidget_clipComponentsToDimensions, defaultValues->componentwidget_clipComponentsToDimensions);

		DescManager::getSingleton().destroyDesc(defaultValues);
	}

	ComponentWidget::ComponentWidget(const Ogre::String& name) :
		Widget(name)
	{
	}

	ComponentWidget::~ComponentWidget()
	{
		WidgetFactory* f = FactoryManager::getSingleton().getWidgetFactory();
		for(std::map<Ogre::String,Widget*>::iterator it = mComponents.begin(); it != mComponents.end(); ++it)
			f->destroyInstance((*it).second);
	}

	void ComponentWidget::_initialize(WidgetDesc* d)
	{
		Widget::_initialize(d);

		ComponentWidgetDesc* cwd = dynamic_cast<ComponentWidgetDesc*>(d);
		setClipComponentsToDimensions(cwd->componentwidget_clipComponentsToDimensions);
	}

	void ComponentWidget::_setGUIManager(GUIManager* gm)
	{
		Widget::_setGUIManager(gm);

		for(std::map<Ogre::String,Widget*>::iterator it = mComponents.begin(); it != mComponents.end(); ++it)
			(*it).second->_setGUIManager(gm);
	}

	void ComponentWidget::_setSheet(Sheet* sheet)
	{
		Widget::_setSheet(sheet);

		for(std::map<Ogre::String,Widget*>::iterator it = mComponents.begin(); it != mComponents.end(); ++it)
			(*it).second->_setSheet(sheet);
	}

	void ComponentWidget::addComponent(const Ogre::String& alias, Widget* w)
	{
		if(mComponents.find(alias) != mComponents.end())
			throw Exception(Exception::ERR_WIDGET_ALREADY_ADDED,"The widget \"" + mWidgetDesc->widget_name + "\" already contains a Component using alias \"" + alias + "\"!","addComponent");

		w->mComponentOfAWidget = true;
		// Component names should not be registered with the Sheet, and aren't visible in terms of name lookups.
		if(w->mNameRegisteredWithSheet && (mWidgetDesc->sheet != NULL))
		{
			mWidgetDesc->sheet->_notifyNameFreed(w->mWidgetDesc->widget_name);
			w->mNameRegisteredWithSheet = false;
		}
		w->setPositionRelativeToParentClientDimensions(false);
		mComponents[alias] = w;
		w->setParent(this);
		w->_setGUIManager(mWidgetDesc->guiManager);
		w->_setSheet(mWidgetDesc->sheet);
	}

	void ComponentWidget::applyAnchor(Widget* child, Size changeInSize)
	{
		switch(child->getHorizontalAnchor())
		{
		case ANCHOR_HORIZONTAL_CENTER:
			{
				// Center vertically
				Point p = child->getPosition();
				p.x = (mWidgetDesc->widget_dimensions.size.width / 2.0) - (child->getWidth() / 2.0);
				child->setPosition(p);
			}
			break;
		case ANCHOR_HORIZONTAL_CENTER_DYNAMIC:
			{
				// grow or shrink the widget according to new dimensions
				child->setWidth(child->getWidth() + changeInSize.width);
				// Center vertically
				Point p = child->getPosition();
				p.x = (mWidgetDesc->widget_dimensions.size.width / 2.0) - (child->getWidth() / 2.0);
				child->setPosition(p);
			}
			break;
		case ANCHOR_HORIZONTAL_LEFT_RIGHT:
			{
				// grow or shrink the widget according to new dimensions
				child->setWidth(child->getWidth() + changeInSize.width);
			}
			break;
		case ANCHOR_HORIZONTAL_RIGHT:
			{
				// re-align component widget according to right edge of this widgets new dimensions
				Point p = child->getPosition();
				p.x += changeInSize.width;
				child->setPosition(p);
			}
			break;
		}

		switch(child->getVerticalAnchor())
		{
		case ANCHOR_VERTICAL_CENTER:
			{
				// Center vertically
				Point p = child->getPosition();
				p.y = (mWidgetDesc->widget_dimensions.size.height / 2.0) - (child->getHeight() / 2.0);
				child->setPosition(p);
			}
			break;
		case ANCHOR_VERTICAL_CENTER_DYNAMIC:
			{
				// grow or shrink the widget according to new dimensions
				child->setHeight(child->getHeight() + changeInSize.height);
				// Center vertically
				Point p = child->getPosition();
				p.y = (mWidgetDesc->widget_dimensions.size.height / 2.0) - (child->getHeight() / 2.0);
				child->setPosition(p);
			}
			break;
		case ANCHOR_VERTICAL_TOP_BOTTOM:
			{
				// grow or shrink the widget according to new dimensions
				child->setHeight(child->getHeight() + changeInSize.height);
			}
			break;
		case ANCHOR_VERTICAL_BOTTOM:
			{
				// re-align component widget according to bottom edge of this widgets new dimensions
				Point p = child->getPosition();
				p.y += changeInSize.height;
				child->setPosition(p);
			}
			break;
		}
	}

	void ComponentWidget::draw()
	{
		// check visibility
		if( !mWidgetDesc->widget_visible )
			return;

		Brush* brush = Brush::getSingletonPtr();

		// check and store clip region
		Rect prevClipRegion = brush->getClipRegion();
		if ( prevClipRegion.getIntersection(Rect(mTexturePosition,mWidgetDesc->widget_dimensions.size)) == Rect::ZERO )
			return;

		// set clip region to dimensions
		brush->setClipRegion(Rect(mTexturePosition,mWidgetDesc->widget_dimensions.size).getIntersection(prevClipRegion));

		// Set color before drawing operations
		brush->setColor(mWidgetDesc->widget_baseColor);

		// Set opacity before drawing operations
		brush->setOpacity(getAbsoluteOpacity());

		// draw self
		onDraw();

		if(!dynamic_cast<ComponentWidgetDesc*>(mWidgetDesc)->componentwidget_clipComponentsToDimensions)
			brush->setClipRegion(prevClipRegion);

		// draw components
		for(std::map<Ogre::String,Widget*>::iterator it = mComponents.begin(); it != mComponents.end(); ++it)
			(*it).second->draw();

		// restore clip region
		brush->setClipRegion(prevClipRegion);
	}

	Widget* ComponentWidget::findWidgetAtPoint(const Point& p, unsigned int queryFilter, bool ignoreDisabled)
	{
		// If we are not widget_visible, return NULL
		if(!mWidgetDesc->widget_visible)
			return NULL;

		if((mWidgetDesc->widget_queryFlags & queryFilter) == 0)
			return NULL;

		// If we ignore disabled and this widget is !widget_enabled, return NULL
		if(ignoreDisabled && !mWidgetDesc->widget_enabled)
			return NULL;

		// Check components before verifying point is within bounds. (Components can lie outside widget dimensions)
		for(std::map<Ogre::String,Widget*>::iterator it = mComponents.begin(); it != mComponents.end(); ++it)
		{
			Widget* w = (*it).second->findWidgetAtPoint(p,queryFilter,ignoreDisabled);
			if(w != NULL)
				return w;
		}

		// See if point is within widget bounds
		if(!Rect(mTexturePosition,mWidgetDesc->widget_dimensions.size).isPointWithinBounds(p))
			return NULL;
				
		// Take transparency picking into account
		if(mWidgetDesc->widget_transparencyPicking && (mSkinType != NULL))
		{
			// If the background is transparent, return NULL
			if((mSkinElement == NULL) || (mSkinElement->getTextureName() == ""))
				return NULL;

			// Get relative position
			Point relPos = p - mTexturePosition;
			// Get percentage of position relative to widget widget_dimensions
			relPos.x = relPos.x / mWidgetDesc->widget_dimensions.size.width;
			relPos.y = relPos.y / mWidgetDesc->widget_dimensions.size.height;
			// Get pixel position of texture
			Point pixelPos;
			pixelPos.x = relPos.x * (mSkinElement->getWidth() - 1);
			pixelPos.y = relPos.y * (mSkinElement->getHeight() - 1);
			
			if(mSkinElement->transparentPixel(pixelPos))
				return NULL;
		}

		return this;
	}

	bool ComponentWidget::getClipComponentsToDimensions()
	{
		return dynamic_cast<ComponentWidgetDesc*>(mWidgetDesc)->componentwidget_clipComponentsToDimensions;
	}

	bool ComponentWidget::isComponentWidget()
	{
		return true;
	}

	void ComponentWidget::removeEventHandlers(void* obj)
	{
		Widget::removeEventHandlers(obj);

		for(std::map<Ogre::String,Widget*>::iterator it = mComponents.begin(); it != mComponents.end(); ++it)
			(*it).second->removeEventHandlers(obj);
	}

	void ComponentWidget::setClipComponentsToDimensions(bool clip)
	{
		dynamic_cast<ComponentWidgetDesc*>(mWidgetDesc)->componentwidget_clipComponentsToDimensions = clip;

		redraw();
	}

	void ComponentWidget::setParent(Widget* parent)
	{
		Widget::setParent(parent);

		// Update components window reference via setParent
		for(std::map<Ogre::String,Widget*>::iterator it = mComponents.begin(); it != mComponents.end(); ++it)
		{
			(*it).second->setParent(this);
		}
	}

	void ComponentWidget::setMoveBaseWidgetOnDrag(bool moveBaseWidget)
	{
		Widget::setMoveBaseWidgetOnDrag(moveBaseWidget);

		for(std::map<Ogre::String,Widget*>::iterator it = mComponents.begin(); it != mComponents.end(); ++it)
			(*it).second->setMoveBaseWidgetOnDrag(moveBaseWidget);
	}

	void ComponentWidget::setSkinType(const Ogre::String type)
	{
		Widget::setSkinType(type);

		for(std::map<Ogre::String,Widget*>::iterator it = mComponents.begin(); it != mComponents.end(); ++it)
		{
			if(mSkinType->hasSkinReference((*it).first))
				(*it).second->setSkinType(mSkinType->getSkinReference((*it).first)->typeName);
		}
	}

	void ComponentWidget::updateClientDimensions()
	{
		Size previousSize = mClientDimensions.size;

		mClientDimensions.position = Point::ZERO;
		mClientDimensions.size = mWidgetDesc->widget_dimensions.size;

		if(mSkinElement != NULL)
		{
			mClientDimensions.position.x = mSkinElement->getBorderThickness(BORDER_LEFT);
			mClientDimensions.position.y = mSkinElement->getBorderThickness(BORDER_TOP);
			mClientDimensions.size.width = mWidgetDesc->widget_dimensions.size.width - (mSkinElement->getBorderThickness(BORDER_LEFT) + mSkinElement->getBorderThickness(BORDER_RIGHT));
			mClientDimensions.size.height = mWidgetDesc->widget_dimensions.size.height - (mSkinElement->getBorderThickness(BORDER_TOP) + mSkinElement->getBorderThickness(BORDER_BOTTOM));
		}

		// Get difference.  A larger size indicates a positive difference
		Size difference = mClientDimensions.size - previousSize;

		// Handle anchoring for Components
		for(std::map<Ogre::String,Widget*>::iterator it = mComponents.begin(); it != mComponents.end(); ++it)
		{
			applyAnchor((*it).second,difference);
		}

		WidgetEventArgs args(this);
		fireWidgetEvent(WIDGET_EVENT_CLIENTSIZE_CHANGED,args);

		redraw();
	}

	void ComponentWidget::updateTexturePosition()
	{
		Widget::updateTexturePosition();

		// Update component screen dimensions, must be done after client and screen rect have been calculated
		for(std::map<Ogre::String,Widget*>::iterator it = mComponents.begin(); it != mComponents.end(); ++it)
		{
			(*it).second->updateTexturePosition();
		}
	}
}
