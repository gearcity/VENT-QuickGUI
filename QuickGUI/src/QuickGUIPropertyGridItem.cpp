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

#include "QuickGUIPropertyGridItem.h"
#include "QuickGUIPropertyGrid.h"
#include "QuickGUIButton.h"
#include "QuickGUIPropertyGridSection.h"
#include "QuickGUIDescManager.h"

namespace QuickGUI
{
	const Ogre::String PropertyGridItem::BACKGROUND = "background";
	const Ogre::String PropertyGridItem::ICON = "icon";
	const Ogre::String PropertyGridItem::SELECTED = "selected";

	PropertyGridItemDesc::PropertyGridItemDesc() :
		ContainerWidgetDesc(),
		TextUserDesc()
	{
		resetToDefault();
	}

	void PropertyGridItemDesc::resetToDefault()
	{
		ContainerWidgetDesc::resetToDefault();

		TextUserDesc::resetToDefault();

		containerwidget_clipChildrenToDimensions = false;
		containerwidget_supportScrollBars = false;

		propertygriditem_index = -1;
		propertygriditem_selected = false;

		propertyGrid = NULL;
	}

	void PropertyGridItemDesc::serialize(SerialBase* b)
	{
		// Retrieve default values to supply to the serial reader/writer.
		// The reader uses the default value if the given property does not exist.
		// The writer does not write out the given property if it has the same value as the default value.
		PropertyGridItemDesc* defaultValues = DescManager::getSingleton().createDesc<PropertyGridItemDesc>(getClass(),"temp");
		defaultValues->resetToDefault();

		b->IO("Index",		&propertygriditem_index,	defaultValues->propertygriditem_index);
		b->IO("Selected",	&propertygriditem_selected, defaultValues->propertygriditem_selected);

		DescManager::getSingleton().destroyDesc(defaultValues);

		TextUserDesc::serialize(b);
	}

	PropertyGridItem::PropertyGridItem(const Ogre::String& name) :
		ContainerWidget(name),
		TextUser(),
		mFieldWidget(NULL),
		mLabelBuffer(10)
	{
		addWidgetEventHandler(WIDGET_EVENT_KEY_DOWN,&PropertyGridItem::onKeyDown,this);
		addWidgetEventHandler(WIDGET_EVENT_MOUSE_BUTTON_DOWN,&PropertyGridItem::onMouseDown,this);
	}

	PropertyGridItem::~PropertyGridItem()
	{
	}

	void PropertyGridItem::_initialize(WidgetDesc* d)
	{
		PropertyGridItemDesc* pgid = dynamic_cast<PropertyGridItemDesc*>(d);
		pgid->widget_consumeKeyboardEvents = true;

		ContainerWidget::_initialize(d);

		mDesc = dynamic_cast<PropertyGridItemDesc*>(mWidgetDesc);

		mDesc->propertygriditem_index = pgid->propertygriditem_index;

		// Make a copy of the Text Desc.  The Text object will
		// modify it directly, which is used for serialization.
		mDesc->textDesc = pgid->textDesc;

		TextUser::_initialize(this,mDesc);

		setSkinType(pgid->widget_skinTypeName);

		if(pgid->propertygriditem_selected)
			select();
	}

	float PropertyGridItem::_getFarthestX()
	{
		if(!mDesc->widget_visible)
			return 0;

		return getPositionRelativeToPropertyGrid().x + mWidgetDesc->widget_dimensions.size.width;
	}

	void PropertyGridItem::_selectProperty(PropertyGridItem* pgp)
	{
		if(pgp == this)
			mDesc->propertygriditem_selected = true;
		else
			mDesc->propertygriditem_selected = false;
	}

	Widget* PropertyGridItem::findWidgetAtPoint(const Point& p, unsigned int queryFilter, bool ignoreDisabled)
	{
		for(std::map<Ogre::String,Widget*>::iterator it = mComponents.begin(); it != mComponents.end(); ++it)
		{
			Widget* w = (*it).second->findWidgetAtPoint(p,queryFilter,ignoreDisabled);
			if(w != NULL)
				return w;
		}

		for(std::list<Widget*>::reverse_iterator it = mChildren.rbegin(); it != mChildren.rend(); ++it)
		{
			Widget* w = (*it)->findWidgetAtPoint(p,queryFilter,ignoreDisabled);
			if(w != NULL)
				return w;
		}

		return Widget::findWidgetAtPoint(p,queryFilter,ignoreDisabled);
	}

	float PropertyGridItem::getBranchHeight()
	{
		float height = 0;

		// If invisible, return 0 height
		if(!mDesc->widget_visible)
			return height;

		// If we made it here we know we're visible, so at the very least include our height
		height += mDesc->widget_dimensions.size.height;

		return height;
	}

	int PropertyGridItem::getIndex()
	{
		return mDesc->propertygriditem_index;
	}

	PropertyGridItem* PropertyGridItem::getLastVisibleItem()
	{
		if(!mDesc->widget_visible)
			return NULL;

		return this;
	}

	PropertyGridItem* PropertyGridItem::getNextSibling()
	{
		if(mParentWidget == NULL)
			return NULL;

		bool returnNext = false;

		std::list<PropertyGridItem*> list = getSiblings();
		for(std::list<PropertyGridItem*>::iterator it = list.begin(); it != list.end(); ++it)
		{
			if(returnNext)
				return (*it);

			if((*it) == this)
				returnNext = true;
		}

		return NULL;
	}

	int PropertyGridItem::getNumberOfSiblings()
	{
		if(mParentWidget != NULL)
		{
			if(mParentWidget->getClass() == "PropertyGrid")
				return dynamic_cast<PropertyGrid*>(mParentWidget)->getNumberOfSections();
			else
				return dynamic_cast<PropertyGridSection*>(mParentWidget)->getNumberOfProperties();
		}

		return 0;
	}

	PropertyGridItem* PropertyGridItem::getPreviousSibling()
	{
		if(mParentWidget == NULL)
			return NULL;

		PropertyGridItem* i = NULL;

		std::list<PropertyGridItem*> list = getSiblings();
		for(std::list<PropertyGridItem*>::iterator it = list.begin(); it != list.end(); ++it)
		{
			if((*it) == this)
				return i;

			i = (*it);
		}

		return NULL;
	}

	Point PropertyGridItem::getPositionRelativeToPropertyGrid()
	{
		Point p(mWidgetDesc->widget_dimensions.position);

		Widget* w = mParentWidget;
		while((w != NULL) && (w->getClass() != "PropertyGrid"))
		{
			p.translate(w->getPosition());

			w = w->getParentWidget();
		}

		return p;
	}

	std::list<PropertyGridItem*> PropertyGridItem::getSiblings()
	{
		std::list<PropertyGridItem*> list;
		if(mParentWidget->getClass() == "PropertyGrid")
		{
			std::list<PropertyGridSection*> sections = dynamic_cast<PropertyGrid*>(mParentWidget)->getRootSections();
			for(std::list<PropertyGridSection*>::iterator it = sections.begin(); it != sections.end(); ++it)
				list.push_back((*it));
		}
		else
			list = dynamic_cast<PropertyGridSection*>(mParentWidget)->getItems();

		return list;
	}

	bool PropertyGridItem::isPositionManaged()
	{
		return true;
	}

	bool PropertyGridItem::isSizeManaged()
	{
		return true;
	}

	void PropertyGridItem::notifyPropertyGridParent(PropertyGrid* g)
	{
		mDesc->propertyGrid = g;

		updateFieldSkin();
	}

	void PropertyGridItem::notifyHeightUpdated()
	{
		if(mDesc->propertyGrid != NULL)
		{
			float itemHeight = mDesc->propertyGrid->getItemHeight();
			setHeight(itemHeight);
			setWidth(itemHeight + dynamic_cast<PropertyGridSection*>(mParentWidget)->mLabelWidth + mLabelBuffer + mDesc->propertyGrid->getPropertyFieldWidth());

			if(mFieldWidget != NULL)
				mFieldWidget->setHeight(itemHeight);
		}
	}

	void PropertyGridItem::notifyWidthUpdated()
	{
		if(mDesc->propertyGrid != NULL)
		{
			// Get the width of the label area of the Item: Icon + PropertyGridLabelWidth.
			float labelWidth = dynamic_cast<PropertyGridSection*>(getParentWidget())->mLabelWidth;
			float fieldWidth = mDesc->propertyGrid->getPropertyFieldWidth();
			setWidth(labelWidth + fieldWidth);

			if(mFieldWidget != NULL)
			{
				Point p = mFieldWidget->getPosition();
				mFieldWidget->setPosition(Point(labelWidth,p.y));
				mFieldWidget->setWidth(fieldWidth);
			}
		}
	}

	void PropertyGridItem::onDraw()
	{
		Brush* brush = Brush::getSingletonPtr();

		brush->setFilterMode(mDesc->widget_brushFilterMode);

		// Draw Background
		Point backgroundPosition(mTexturePosition);
		brush->drawSkinElement(Rect(backgroundPosition,mWidgetDesc->widget_dimensions.size),mSkinElement);

		// if Selected, draw Selected SkinElement
		if(mDesc->propertygriditem_selected)
			brush->drawSkinElement(Rect(backgroundPosition,mWidgetDesc->widget_dimensions.size),mSkinType->getSkinElement(SELECTED));

		ColourValue prevColor = brush->getColour();
		Rect prevClipRegion = brush->getClipRegion();

		// Draw Icon
		Point iconPosition(mTexturePosition.x,mTexturePosition.y);
		brush->drawSkinElement(Rect(iconPosition,Size(mWidgetDesc->widget_dimensions.size.height)),mSkinType->getSkinElement(ICON));

		// Center Text Vertically

		float textHeight = mText->getTextHeight();
		float yPos = (mDesc->widget_dimensions.size.height / 2.0) - (textHeight / 2.0);

		// Clip to client dimensions
		Rect clipRegion;
		if(mFieldWidget != NULL)
			clipRegion = Rect(mTexturePosition,Size((mWidgetDesc->widget_dimensions.size.width - mFieldWidget->getWidth()) - mWidgetDesc->widget_dimensions.size.height,mWidgetDesc->widget_dimensions.size.height));
		else
			clipRegion = Rect(mTexturePosition,Size(mWidgetDesc->widget_dimensions.size.width - mWidgetDesc->widget_dimensions.size.height,mWidgetDesc->widget_dimensions.size.height));
		clipRegion.translate(Point(mWidgetDesc->widget_dimensions.size.height,0));

		brush->setClipRegion(prevClipRegion.getIntersection(clipRegion));

		// Adjust Rect to Text drawing region
		Point textPosition = clipRegion.position;
		textPosition.y += yPos;

		mText->draw(textPosition);

		// Restore Clipping
		brush->setClipRegion(prevClipRegion);
		brush->setColor(prevColor);
	}

	void PropertyGridItem::onKeyDown(const EventArgs& args)
	{
		if((mDesc->propertyGrid != NULL) && (!mDesc->propertyGrid->getKeyNavigation()))
			return;

		const KeyEventArgs& kea = dynamic_cast<const KeyEventArgs&>(args);

		switch(kea.scancode)
		{
		case KC_LEFT:
			if((mParentWidget != NULL) && (mParentWidget->getClass() != "PropertyGrid"))
				mDesc->propertyGrid->selectItem(dynamic_cast<PropertyGridItem*>(mParentWidget));
			break;
		case KC_UP:
			// If we are the first child node
			if(mDesc->propertygriditem_index == 0) 
			{
				// If we are not root nodes, select our parent
				if(mParentWidget->getClass() != "PropertyGrid")
					mDesc->propertyGrid->selectItem(dynamic_cast<PropertyGridItem*>(mParentWidget));
			}
			else
				mDesc->propertyGrid->selectItem(getPreviousSibling()->getLastVisibleItem());
			break;
		case KC_DOWN:
			{
				// Determine if we are the last sibling
			
				int numSiblings = 0;
				if(mParentWidget != NULL)
				{
					if(mParentWidget->getClass() == "PropertyGrid")
						numSiblings = dynamic_cast<PropertyGrid*>(mParentWidget)->getNumberOfSections();
					else
						numSiblings = dynamic_cast<PropertyGridSection*>(mParentWidget)->getNumberOfProperties();
				}

				// If we are the last child node, iterate up through parents and select the next sibling if exists
				if(mDesc->propertygriditem_index == (numSiblings - 1))
				{
					Widget* w = mParentWidget;
					while(w->getClass() != "PropertyGrid")
					{
						PropertyGridItem* i = dynamic_cast<PropertyGridItem*>(w)->getNextSibling();

						if(i != NULL)
						{
							mDesc->propertyGrid->selectItem(i);
							break;
						}

						w = w->getParentWidget();
					}
				}
				// If there are siblings below us, just select the next sibling
				else
					mDesc->propertyGrid->selectItem(getNextSibling());
			}
			break;
		}
	}

	void PropertyGridItem::onMouseDown(const EventArgs& args)
	{
		const MouseEventArgs& mea = dynamic_cast<const MouseEventArgs&>(args);

		if(mea.button == MB_Left)
			select();
	}

	void PropertyGridItem::onTextChanged()
	{
		if(mDesc->propertyGrid != NULL)
		{
			dynamic_cast<PropertyGridSection*>(getParentWidget())->updateLabelWidth(mDesc->propertyGrid->getItemHeight() + getTextWidth() + mLabelBuffer);
		}
	}

	void PropertyGridItem::select()
	{
		if(mDesc->propertyGrid != NULL)
			mDesc->propertyGrid->selectItem(this);
	}

	void PropertyGridItem::updateItems()
	{
		// Does nothing
	}

	void PropertyGridItem::updateFieldSkin()
	{
		// Does nothing
	}
}
