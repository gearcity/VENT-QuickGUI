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

#include "QuickGUIPropertyGridComboBoxProperty.h"
#include "QuickGUISkinDefinitionManager.h"
#include "QuickGUIPropertyGrid.h"
#include "QuickGUISkinDefinition.h"
#include "QuickGUIComboBox.h"
#include "QuickGUIListTextItem.h"
#include "QuickGUIListImageItem.h"
#include "QuickGUIDescManager.h"

namespace QuickGUI
{
	void PropertyGridComboBoxProperty::registerSkinDefinition()
	{
		SkinDefinition* d = OGRE_NEW_T(SkinDefinition,Ogre::MEMCATEGORY_GENERAL)("PropertyGridComboBoxProperty");
		d->defineSkinElement(BACKGROUND);
		d->defineSkinElement(ICON);
		d->defineSkinElement(SELECTED);
		d->definitionComplete();

		SkinDefinitionManager::getSingleton().registerSkinDefinition("PropertyGridComboBoxProperty",d);
	}

	PropertyGridComboBoxPropertyDesc::PropertyGridComboBoxPropertyDesc() :
		PropertyGridItemDesc()
	{
		resetToDefault();
	}

	void PropertyGridComboBoxPropertyDesc::resetToDefault()
	{
		PropertyGridItemDesc::resetToDefault();
	}

	void PropertyGridComboBoxPropertyDesc::serialize(SerialBase* b)
	{
		PropertyGridItemDesc::serialize(b);
	}

	PropertyGridComboBoxProperty::PropertyGridComboBoxProperty(const Ogre::String& name) :
		PropertyGridItem(name),
		mComboBox(NULL)
	{
	}

	PropertyGridComboBoxProperty::~PropertyGridComboBoxProperty()
	{
	}

	void PropertyGridComboBoxProperty::_initialize(WidgetDesc* d)
	{		
		PropertyGridItem::_initialize(d);

		mDesc = dynamic_cast<PropertyGridComboBoxPropertyDesc*>(mWidgetDesc);

		ComboBoxDesc* cbd = DescManager::getSingletonPtr()->getDefaultComboBoxDesc();
		cbd->resetToDefault();
		cbd->widget_dimensions.position.x = (mWidgetDesc->widget_dimensions.size.width / 2.0);
		cbd->widget_dimensions.size.width = (mWidgetDesc->widget_dimensions.size.width / 2.0);
		cbd->widget_dimensions.size.height = mWidgetDesc->widget_dimensions.size.height;
		cbd->widget_horizontalAnchor = ANCHOR_HORIZONTAL_RIGHT;

		mComboBox = dynamic_cast<ComboBox*>(_createWidget(cbd));
		addComponent("ComboBoxField",mComboBox);

		mFieldWidget = mComboBox;
	}

	ListImageItem* PropertyGridComboBoxProperty::createImageItem(const Ogre::String& imageName, int index)
	{
		return mComboBox->createImageItem(imageName,index);
	}

	ListImageItem* PropertyGridComboBoxProperty::createImageItem(int index)
	{
		return mComboBox->createImageItem(index);
	}

	ListTextItem* PropertyGridComboBoxProperty::createTextItem(const Ogre::UTFString& text, int index)
	{
		return mComboBox->createTextItem(text,index);
	}

	ListTextItem* PropertyGridComboBoxProperty::createTextItem(std::vector<TextSegment> segments, int index)
	{
		return mComboBox->createTextItem(segments,index);
	}

	ListTextItem* PropertyGridComboBoxProperty::createTextItem(int index)
	{
		return mComboBox->createTextItem(index);
	}

	void PropertyGridComboBoxProperty::destroyItem(unsigned int index)
	{
		mComboBox->destroyItem(index);
	}
	
	void PropertyGridComboBoxProperty::destroyItem(ListItem* i)
	{
		mComboBox->destroyItem(i);
	}

	Ogre::String PropertyGridComboBoxProperty::getClass()
	{
		return "PropertyGridComboBoxProperty";
	}

	float PropertyGridComboBoxProperty::getDropDownWidth()
	{
		return mComboBox->getDropDownWidth();
	}

	ListItem* PropertyGridComboBoxProperty::getItem(unsigned int index)
	{
		return mComboBox->getItem(index);
	}

	float PropertyGridComboBoxProperty::getMaxDropDownHeight()
	{
		return mComboBox->getMaxDropDownHeight();
	}

	int PropertyGridComboBoxProperty::getNumberOfItems()
	{
		return mComboBox->getNumberOfItems();
	}

	ListItem* PropertyGridComboBoxProperty::getSelectedItem()
	{
		return mComboBox->getSelectedItem();
	}

	void PropertyGridComboBoxProperty::hideDropDownList()
	{
		mComboBox->hideDropDownList();
	}

	void PropertyGridComboBoxProperty::selectItem(unsigned int index)
	{
		mComboBox->selectItem(index);
	}

	void PropertyGridComboBoxProperty::setDropDownWidth(float width)
	{
		mComboBox->setDropDownWidth(width);
	}

	void PropertyGridComboBoxProperty::setMaxDropDownHeight(float height)
	{
		mComboBox->setMaxDropDownHeight(height);
	}

	void PropertyGridComboBoxProperty::showDropDownList()
	{
		mComboBox->showDropDownList();
	}

	void PropertyGridComboBoxProperty::updateFieldSkin()
	{
		if((mDesc->propertyGrid != NULL) && (mComboBox != NULL))
			mComboBox->setSkinType(dynamic_cast<PropertyGrid*>(mDesc->propertyGrid)->getComboBoxFieldSkin());
	}
}
