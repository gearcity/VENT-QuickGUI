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

#include "QuickGUIPropertyGridBoolProperty.h"
#include "QuickGUISkinDefinitionManager.h"
#include "QuickGUIFactoryManager.h"
#include "QuickGUIPropertyGrid.h"
#include "QuickGUIListTextItem.h"
#include "QuickGUISkinDefinition.h"
#include "QuickGUIComboBox.h"
#include "QuickGUIDescManager.h"

namespace QuickGUI
{
	void PropertyGridBoolProperty::registerSkinDefinition()
	{
		SkinDefinition* d = OGRE_NEW_T(SkinDefinition,Ogre::MEMCATEGORY_GENERAL)("PropertyGridBoolProperty");
		d->defineSkinElement(BACKGROUND);
		d->defineSkinElement(ICON);
		d->defineSkinElement(SELECTED);
		d->definitionComplete();

		SkinDefinitionManager::getSingleton().registerSkinDefinition("PropertyGridBoolProperty",d);
	}

	PropertyGridBoolPropertyDesc::PropertyGridBoolPropertyDesc() :
		PropertyGridItemDesc()
	{
		resetToDefault();
	}

	void PropertyGridBoolPropertyDesc::resetToDefault()
	{
		PropertyGridItemDesc::resetToDefault();
	}

	void PropertyGridBoolPropertyDesc::serialize(SerialBase* b)
	{
		PropertyGridItemDesc::serialize(b);
	}

	PropertyGridBoolProperty::PropertyGridBoolProperty(const Ogre::String& name) :
		PropertyGridItem(name),
		mComboBox(NULL)
	{
	}

	PropertyGridBoolProperty::~PropertyGridBoolProperty()
	{
	}

	void PropertyGridBoolProperty::_initialize(WidgetDesc* d)
	{		
		PropertyGridItem::_initialize(d);

		mDesc = dynamic_cast<PropertyGridBoolPropertyDesc*>(mWidgetDesc);

		ComboBoxDesc* cbd = DescManager::getSingletonPtr()->getDefaultComboBoxDesc();
		cbd->resetToDefault();
		cbd->widget_dimensions.position.x = (mWidgetDesc->widget_dimensions.size.width / 2.0);
		cbd->widget_dimensions.size.width = (mWidgetDesc->widget_dimensions.size.width / 2.0);
		cbd->widget_dimensions.size.height = mWidgetDesc->widget_dimensions.size.height;
		cbd->widget_horizontalAnchor = ANCHOR_HORIZONTAL_RIGHT;

		mComboBox = dynamic_cast<ComboBox*>(_createWidget(cbd));
		addComponent("ComboBoxField",mComboBox);

		ListTextItemDesc* ltid = DescManager::getSingletonPtr()->getDefaultListTextItemDesc();
		ltid->resetToDefault();
		ltid->textDesc.segments.push_back(TextSegment("micross.12",ColourValue::White,"False"));
		mComboBox->createItem(ltid);

		ltid->resetToDefault();
		ltid->textDesc.segments.push_back(TextSegment("micross.12",ColourValue::White,"True"));
		mComboBox->createItem(ltid);

		mComboBox->selectItem(0);

		mFieldWidget = mComboBox;
	}

	Ogre::String PropertyGridBoolProperty::getClass()
	{
		return "PropertyGridBoolProperty";
	}

	bool PropertyGridBoolProperty::getProperty()
	{
		if(mComboBox->getSelectedItem()->getIndex() == 0)
			return false;
		else
			return true;
	}

	void PropertyGridBoolProperty::setProperty(bool b)
	{
		if(b)
			mComboBox->selectItem(1);
		else
			mComboBox->selectItem(0);
	}

	void PropertyGridBoolProperty::updateFieldSkin()
	{
		if((mDesc->propertyGrid != NULL) && (mComboBox != NULL))
			mComboBox->setSkinType(dynamic_cast<PropertyGrid*>(mDesc->propertyGrid)->getBoolFieldSkin());
	}
}
