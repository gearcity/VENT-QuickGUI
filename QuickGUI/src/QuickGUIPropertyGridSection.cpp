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

#include "QuickGUIPropertyGridSection.h"
#include "QuickGUISkinDefinitionManager.h"
#include "QuickGUISkinDefinition.h"
#include "QuickGUIPropertyGrid.h"
#include "QuickGUIPropertyGridBoolProperty.h"
#include "QuickGUIPropertyGridComboBoxProperty.h"
#include "QuickGUIPropertyGridTextProperty.h"
#include "QuickGUIButton.h"
#include "QuickGUIFactoryManager.h"
#include "QuickGUIDescManager.h"

namespace QuickGUI
{
	void PropertyGridSection::registerSkinDefinition()
	{
		SkinDefinition* d = OGRE_NEW_T(SkinDefinition,Ogre::MEMCATEGORY_GENERAL)("PropertyGridSection");
		d->defineSkinElement(BACKGROUND);
		d->defineSkinElement(ICON);
		d->defineSkinElement(SELECTED);
		d->definitionComplete();

		SkinDefinitionManager::getSingleton().registerSkinDefinition("PropertyGridSection",d);
	}

	PropertyGridSectionDesc::PropertyGridSectionDesc() :
		PropertyGridItemDesc()
	{
		resetToDefault();
	}

	void PropertyGridSectionDesc::resetToDefault()
	{
		PropertyGridItemDesc::resetToDefault();
	}

	void PropertyGridSectionDesc::serialize(SerialBase* b)
	{
		PropertyGridItemDesc::serialize(b);
	}

	PropertyGridSection::PropertyGridSection(const Ogre::String& name) :
		PropertyGridItem(name),
		mStateButton(NULL),
		mExpanded(false),
		mLabelWidth(50)
	{
	}

	PropertyGridSection::~PropertyGridSection()
	{
	}

	void PropertyGridSection::_initialize(WidgetDesc* d)
	{		
		PropertyGridItem::_initialize(d);

		mDesc = dynamic_cast<PropertyGridSectionDesc*>(mWidgetDesc);
	}

	float PropertyGridSection::_getFarthestX()
	{
		if(!mDesc->widget_visible)
			return 0;

		float farthestX = getPositionRelativeToPropertyGrid().x + mWidgetDesc->widget_dimensions.size.width;
		float childX = 0;

		for(std::list<PropertyGridItem*>::iterator it = mItems.begin(); it != mItems.end(); ++it)
		{
			childX = (*it)->_getFarthestX();
			if(childX > farthestX)
				farthestX = childX;
		}

		return farthestX;
	}

	void PropertyGridSection::_selectProperty(PropertyGridItem* pgp)
	{
		if(pgp == this)
			mDesc->propertygriditem_selected = true;
		else
			mDesc->propertygriditem_selected = false;

		for(std::list<PropertyGridItem*>::iterator it = mItems.begin(); it != mItems.end(); ++it)
			(*it)->_selectProperty(pgp);
	}

	void PropertyGridSection::addChild(Widget* w)
	{
		// Try to be efficient and only create the button once we have child nodes.
		createStateButtonIfNotExists();

		ContainerWidget::addChild(w);

		PropertyGridItem* i = dynamic_cast<PropertyGridItem*>(w);

		// Set link to TreeView
		i->notifyPropertyGridParent(mDesc->propertyGrid);

		i->setPositionRelativeToParentClientDimensions(false);
		i->setVisible(false);

		if(mDesc->propertyGrid != NULL)
		{
			// Update dimensions
			float itemHeight = mDesc->propertyGrid->getItemHeight();
			i->setHeight(itemHeight);

			// Property Grid Section has an expand button, icon, and text. Button and Icon have same height/width, which is the height of the item.
			if(i->getClass() == "PropertyGridSection")
				i->setWidth((mDesc->propertyGrid->getItemHeight() * 2) + i->getTextWidth() + i->mLabelBuffer);
			else
			{
				float fieldWidth = mDesc->propertyGrid->getPropertyFieldWidth();
				i->setWidth(mLabelWidth + fieldWidth);

				if(i->mFieldWidget != NULL)
				{
					i->mFieldWidget->setPosition(Point(mLabelWidth,0));
					i->mFieldWidget->setSize(Size(fieldWidth,itemHeight));
				}
			}
		}

		int index = i->getIndex();

		// If we're adding to the end, things are simple - don't have to update other node names, or positions.
		if((index < 0) || (index >= static_cast<int>(mItems.size())))
		{
			mItems.push_back(i);
		}
		else
		{		
			// If we've made it here, we have to insert the node into the list, and update all node indices and positions.
			int counter = 0;
			for(std::list<PropertyGridItem*>::iterator it = mItems.begin(); it != mItems.end(); ++it)
			{
				if(counter == index)
				{
					mItems.insert(it,i);
					break;
				}

				++counter;
			}
		}

		// See if LabelWidth should be updated
		if((mDesc->propertyGrid != NULL) && (i->getClass() != "PropertyGridSection"))
			updateLabelWidth(mDesc->propertyGrid->getItemHeight() + i->getTextWidth() + i->mLabelBuffer);
		// See if ItemHeight should be updated
		if(mDesc->propertyGrid != NULL)
		{
			float textHeight = i->getTextHeight();
			if(textHeight > mDesc->propertyGrid->getItemHeight())
				mDesc->propertyGrid->setItemHeight(textHeight);
		}

		updateItems();
	}

	void PropertyGridSection::createStateButtonIfNotExists()
	{
		if(mStateButton != NULL)
			return;

		ButtonDesc* bd = DescManager::getSingletonPtr()->getDefaultButtonDesc();
		bd->resetToDefault();
		bd->widget_dimensions.size.width = mWidgetDesc->widget_dimensions.size.height;
		bd->widget_dimensions.size.height = mWidgetDesc->widget_dimensions.size.height;

		mStateButton = dynamic_cast<Button*>(_createWidget(bd));
		mStateButton->addWidgetEventHandler(WIDGET_EVENT_MOUSE_BUTTON_DOWN,&PropertyGridSection::onStateButtonClicked,this);
		addComponent("StateButton",mStateButton);

		if(mDesc->propertyGrid != NULL)
		{
			if(mExpanded)
				mStateButton->setSkinType(mDesc->propertyGrid->getExpandSkinTypeName());
			else
				mStateButton->setSkinType(mDesc->propertyGrid->getMinimizeSkinTypeName());
		}
	}

	PropertyGridItem* PropertyGridSection::createItem(PropertyGridItemDesc* d, int index)
	{
		d->propertygriditem_index = index;

		PropertyGridItem* newItem = dynamic_cast<PropertyGridItem*>(_createWidget(d));

		addChild(newItem);

		return newItem;
	}

	PropertyGridTextProperty* PropertyGridSection::createTextProperty(const Ogre::UTFString& text, int index)
	{
		PropertyGridTextPropertyDesc* d = DescManager::getSingletonPtr()->getDefaultPropertyGridTextPropertyDesc();
		d->resetToDefault();
		d->textDesc.segments.push_back(TextSegment(text));

		PropertyGridTextProperty* p = dynamic_cast<PropertyGridTextProperty*>(createItem(d,index));

		return p;
	}

	PropertyGridTextProperty* PropertyGridSection::createTextProperty(std::vector<TextSegment> segments, int index)
	{
		PropertyGridTextPropertyDesc* d = DescManager::getSingletonPtr()->getDefaultPropertyGridTextPropertyDesc();
		d->resetToDefault();
		d->textDesc.segments = segments;

		PropertyGridTextProperty* p = dynamic_cast<PropertyGridTextProperty*>(createItem(d,index));

		return p;
	}

	PropertyGridTextProperty* PropertyGridSection::createTextProperty(int index)
	{
		return createTextProperty("TextProperty",index);
	}

	PropertyGridBoolProperty* PropertyGridSection::createBoolProperty(const Ogre::UTFString& text, int index)
	{
		PropertyGridBoolPropertyDesc* d = DescManager::getSingletonPtr()->getDefaultPropertyGridBoolPropertyDesc();
		d->resetToDefault();
		d->textDesc.segments.push_back(TextSegment(text));

		PropertyGridBoolProperty* p = dynamic_cast<PropertyGridBoolProperty*>(createItem(d,index));

		return p;
	}

	PropertyGridBoolProperty* PropertyGridSection::createBoolProperty(std::vector<TextSegment> segments, int index)
	{
		PropertyGridBoolPropertyDesc* d = DescManager::getSingletonPtr()->getDefaultPropertyGridBoolPropertyDesc();
		d->resetToDefault();
		d->textDesc.segments = segments;

		PropertyGridBoolProperty* p = dynamic_cast<PropertyGridBoolProperty*>(createItem(d,index));

		return p;
	}

	PropertyGridBoolProperty* PropertyGridSection::createBoolProperty(int index)
	{
		return createBoolProperty("BoolProperty",index);
	}

	PropertyGridComboBoxProperty* PropertyGridSection::createComboBoxProperty(const Ogre::UTFString& text, int index)
	{
		PropertyGridComboBoxPropertyDesc* d = DescManager::getSingletonPtr()->getDefaultPropertyGridComboBoxPropertyDesc();
		d->resetToDefault();
		d->textDesc.segments.push_back(TextSegment(text));

		PropertyGridComboBoxProperty* p = dynamic_cast<PropertyGridComboBoxProperty*>(createItem(d,index));

		return p;
	}

	PropertyGridComboBoxProperty* PropertyGridSection::createComboBoxProperty(std::vector<TextSegment> segments, int index)
	{
		PropertyGridComboBoxPropertyDesc* d = DescManager::getSingletonPtr()->getDefaultPropertyGridComboBoxPropertyDesc();
		d->resetToDefault();
		d->textDesc.segments = segments;

		PropertyGridComboBoxProperty* p = dynamic_cast<PropertyGridComboBoxProperty*>(createItem(d,index));

		return p;
	}

	PropertyGridComboBoxProperty* PropertyGridSection::createComboBoxProperty(int index)
	{
		return createComboBoxProperty("ComboBoxProperty",index);
	}

	PropertyGridSection* PropertyGridSection::createSection(const Ogre::UTFString& text, int index)
	{
		PropertyGridSectionDesc* d = DescManager::getSingletonPtr()->getDefaultPropertyGridSectionDesc();
		d->resetToDefault();
		d->textDesc.segments.push_back(TextSegment(text));

		PropertyGridSection* p = dynamic_cast<PropertyGridSection*>(createItem(d,index));

		d->resetToDefault();

		return p;
	}

	PropertyGridSection* PropertyGridSection::createSection(std::vector<TextSegment> segments, int index)
	{
		PropertyGridSectionDesc* d = DescManager::getSingletonPtr()->getDefaultPropertyGridSectionDesc();
		d->resetToDefault();
		d->textDesc.segments = segments;

		PropertyGridSection* p = dynamic_cast<PropertyGridSection*>(createItem(d,index));

		d->resetToDefault();

		return p;
	}

	PropertyGridSection* PropertyGridSection::createSection(int index)
	{
		return createSection("Section",index);
	}

	void PropertyGridSection::expand()
	{
		// Iterate through parents and make sure they're visible
		Widget* parent = mParentWidget;
		while((parent != NULL) && (parent->getClass() != "PropertyGrid"))
		{
			parent->setVisible(true);

			parent = parent->getParentWidget();
		}

		if(!mItems.empty())
		{
			for(std::list<PropertyGridItem*>::iterator it = mItems.begin(); it != mItems.end(); ++it)
				(*it)->setVisible(true);

			mExpanded = true;

			if(mDesc->propertyGrid != NULL)
				mStateButton->setSkinType(mDesc->propertyGrid->getExpandSkinTypeName());
		}

		// Update Node positions
		if(mDesc->propertyGrid != NULL)
			mDesc->propertyGrid->updateItems();
	}

	float PropertyGridSection::getBranchHeight()
	{
		float height = 0;

		// If invisible, return 0 height
		if(!mDesc->widget_visible)
			return height;

		// If we made it here we know we're visible, so at the very least include our height
		height += mDesc->widget_dimensions.size.height;

		// Add height of child branches (recursive)
		for(std::list<PropertyGridItem*>::iterator it = mItems.begin(); it != mItems.end(); ++it)
			height += (*it)->getBranchHeight();

		return height;
	}

	Ogre::String PropertyGridSection::getClass()
	{
		return "PropertyGridSection";
	}

	std::list<PropertyGridItem*> PropertyGridSection::getItems()
	{
		return mItems;
	}

	PropertyGridItem* PropertyGridSection::getLastVisibleItem()
	{
		if(!mDesc->widget_visible)
			return NULL;

		// If we have no child nodes or are minimized, return this
		if(!mExpanded || mItems.empty())
			return this;

		// If there are visible child nodes, make recursive call using last node in list
		return mItems.back()->getLastVisibleItem();
	}

	int PropertyGridSection::getNumberOfProperties()
	{
		return static_cast<int>(mItems.size());
	}

	void PropertyGridSection::minimize()
	{
		if(mItems.empty())
			return;

		for(std::list<PropertyGridItem*>::iterator it = mItems.begin(); it != mItems.end(); ++it)
			(*it)->setVisible(false);

		mExpanded = false;

		if(mDesc->propertyGrid != NULL)
		{
			mStateButton->setSkinType(mDesc->propertyGrid->getMinimizeSkinTypeName());
			mDesc->propertyGrid->updateItems();
		}
	}

	void PropertyGridSection::notifyPropertyGridParent(PropertyGrid* g)
	{
		mDesc->propertyGrid = g;

		if(g != NULL)
		{
			// Update Button SkinType
			if(mStateButton != NULL)
			{
				if(mExpanded)
					mStateButton->setSkinType(g->getExpandSkinTypeName());
				else
					mStateButton->setSkinType(g->getMinimizeSkinTypeName());
			}
		}

		for(std::list<PropertyGridItem*>::iterator it = mItems.begin(); it != mItems.end(); ++it)
			(*it)->notifyPropertyGridParent(g);
	}

	void PropertyGridSection::notifyHeightUpdated()
	{
		if(mDesc->propertyGrid != NULL)
		{
			float itemHeight = mDesc->propertyGrid->getItemHeight();
			setHeight(itemHeight);
			setWidth((itemHeight * 2) + getTextWidth() + mLabelBuffer);

			for(std::list<PropertyGridItem*>::iterator it = mItems.begin(); it != mItems.end(); ++it)
				(*it)->notifyHeightUpdated();

			updateItems();
		}
	}

	void PropertyGridSection::notifyWidthUpdated()
	{
		for(std::list<PropertyGridItem*>::iterator it = mItems.begin(); it != mItems.end(); ++it)
		{
			// Skip Sections
			if((*it)->getClass() != "PropertyGridSection")
				(*it)->notifyWidthUpdated();
		}
	}

	void PropertyGridSection::onDraw()
	{
		Brush* brush = Brush::getSingletonPtr();

		brush->setFilterMode(mDesc->widget_brushFilterMode);

		// Draw Background
		Point backgroundPosition(mTexturePosition);
		backgroundPosition.x += mWidgetDesc->widget_dimensions.size.height;
		brush->drawSkinElement(Rect(backgroundPosition,mWidgetDesc->widget_dimensions.size),mSkinElement);

		// if Selected, draw Selected SkinElement
		if(mDesc->propertygriditem_selected)
			brush->drawSkinElement(Rect(backgroundPosition,mWidgetDesc->widget_dimensions.size),mSkinType->getSkinElement(SELECTED));

		ColourValue prevColor = brush->getColour();
		Rect prevClipRegion = brush->getClipRegion();

		// Draw Icon
		Point iconPosition(mTexturePosition.x + mWidgetDesc->widget_dimensions.size.height,mTexturePosition.y);
		brush->drawSkinElement(Rect(iconPosition,Size(mWidgetDesc->widget_dimensions.size.height)),mSkinType->getSkinElement(ICON));

		// Center Text Vertically

		float textHeight = mText->getTextHeight();
		float yPos = (mDesc->widget_dimensions.size.height / 2.0) - (textHeight / 2.0);

		// Clip to client dimensions
		Rect clipRegion(mTexturePosition,Size(mWidgetDesc->widget_dimensions.size.width - mWidgetDesc->widget_dimensions.size.height,mWidgetDesc->widget_dimensions.size.height));
		clipRegion.translate(Point(mWidgetDesc->widget_dimensions.size.height * 2,0));

		brush->setClipRegion(prevClipRegion.getIntersection(clipRegion));

		// Adjust Rect to Text drawing region
		Point textPosition = clipRegion.position;
		textPosition.y += yPos;		

		mText->draw(textPosition);

		// Restore Clipping
		brush->setClipRegion(prevClipRegion);
		brush->setColor(prevColor);
	}

	void PropertyGridSection::onKeyDown(const EventArgs& args)
	{
		if((mDesc->propertyGrid != NULL) && (!mDesc->propertyGrid->getKeyNavigation()))
			return;

		const KeyEventArgs& kea = dynamic_cast<const KeyEventArgs&>(args);

		switch(kea.scancode)
		{
		case KC_LEFT:
			if(mExpanded)
				minimize();
			else
			{
				if((mParentWidget != NULL) && (mParentWidget->getClass() != "PropertyGrid"))
					mDesc->propertyGrid->selectItem(dynamic_cast<PropertyGridItem*>(mParentWidget));
			}
			break;
		case KC_RIGHT:
			if(!mExpanded)
				expand();
			else
			{
				if(static_cast<int>(mItems.size()) > 0)
					mDesc->propertyGrid->selectItem(mItems.front());
			}
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
				if(mExpanded)
					mDesc->propertyGrid->selectItem(mItems.front());
				else
				{
					// Determine if we are the last sibling
				
					int numSiblings = 0;
					if(mParentWidget != NULL)
					{
						if(mParentWidget->getClass() == "PropertyGrid")
							numSiblings = dynamic_cast<PropertyGrid*>(mParentWidget)->getNumberOfSections();
						else
							numSiblings = dynamic_cast<PropertyGridSection*>(mParentWidget)->getNumberOfSiblings();
					}

					// If we are the last child node, iterate up through parents and select the next sibling if exists
					if(mDesc->propertygriditem_index == (numSiblings - 1))
					{
						Widget* w = mParentWidget;
						while(w->getClass() != "PropertyGrid")
						{
							PropertyGridItem* i = dynamic_cast<PropertyGridSection*>(w)->getNextSibling();

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
			}
			break;
		}
	}

	void PropertyGridSection::onStateButtonClicked(const EventArgs& args)
	{
		const MouseEventArgs& mea = dynamic_cast<const MouseEventArgs&>(args);

		if((mea.button == MB_Left) && !(mea.autoRepeat))
			toggle();
	}

	void PropertyGridSection::toggle()
	{
		if(mExpanded)
			minimize();
		else
			expand();
	}

	void PropertyGridSection::updateClientDimensions()
	{
		ContainerWidget::updateClientDimensions();

		if(mStateButton != NULL)
		{
			mStateButton->setWidth(mDesc->widget_dimensions.size.height);
			mStateButton->setHeight(mDesc->widget_dimensions.size.height);
		}
	}

	void PropertyGridSection::updateItems()
	{
		float x = mDesc->widget_dimensions.size.height;
		float y = mDesc->widget_dimensions.size.height;
		unsigned int counter = 0;
		for(std::list<PropertyGridItem*>::iterator it = mItems.begin(); it != mItems.end(); ++it)
		{
			(*it)->setPosition(Point(x,y));
			(*it)->mDesc->propertygriditem_index = counter;
			(*it)->updateItems();

			y += (*it)->getBranchHeight();
			++counter;
		}
	}

	void PropertyGridSection::updatePropertySkin()
	{
		for(std::list<PropertyGridItem*>::iterator it = mItems.begin(); it != mItems.end(); ++it)
			(*it)->updateFieldSkin();
	}

	void PropertyGridSection::updateLabelWidth(float width)
	{
		if(width > mLabelWidth)
		{
			mLabelWidth = width;

			for(std::list<PropertyGridItem*>::iterator it = mItems.begin(); it != mItems.end(); ++it)
				(*it)->notifyWidthUpdated();
		}
	}
}
