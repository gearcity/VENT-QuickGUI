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

#include "QuickGUIPropertyGridTextProperty.h"
#include "QuickGUISkinDefinitionManager.h"
#include "QuickGUIFactoryManager.h"
#include "QuickGUIPropertyGrid.h"
#include "QuickGUISkinDefinition.h"
#include "QuickGUITextBox.h"
#include "QuickGUIDescManager.h"

namespace QuickGUI
{
	void PropertyGridTextProperty::registerSkinDefinition()
	{
		SkinDefinition* d = OGRE_NEW_T(SkinDefinition,Ogre::MEMCATEGORY_GENERAL)("PropertyGridTextProperty");
		d->defineSkinElement(BACKGROUND);
		d->defineSkinElement(ICON);
		d->defineSkinElement(SELECTED);
		d->definitionComplete();

		SkinDefinitionManager::getSingleton().registerSkinDefinition("PropertyGridTextProperty",d);
	}

	PropertyGridTextPropertyDesc::PropertyGridTextPropertyDesc() :
		PropertyGridItemDesc()
	{
		resetToDefault();
	}

	void PropertyGridTextPropertyDesc::resetToDefault()
	{
		PropertyGridItemDesc::resetToDefault();
	}

	void PropertyGridTextPropertyDesc::serialize(SerialBase* b)
	{
		PropertyGridItemDesc::serialize(b);
	}

	PropertyGridTextProperty::PropertyGridTextProperty(const Ogre::String& name) :
		PropertyGridItem(name),
		mTextBox(NULL)
	{
	}

	PropertyGridTextProperty::~PropertyGridTextProperty()
	{
	}

	void PropertyGridTextProperty::_initialize(WidgetDesc* d)
	{		
		PropertyGridItem::_initialize(d);

		mDesc = dynamic_cast<PropertyGridTextPropertyDesc*>(mWidgetDesc);

		TextBoxDesc* tbd = DescManager::getSingletonPtr()->getDefaultTextBoxDesc();
		tbd->resetToDefault();
		tbd->widget_dimensions.position.x = (mWidgetDesc->widget_dimensions.size.width / 2.0);
		tbd->widget_dimensions.size.width = (mWidgetDesc->widget_dimensions.size.width / 2.0);
		tbd->widget_dimensions.size.height = mWidgetDesc->widget_dimensions.size.height;
		tbd->widget_horizontalAnchor = ANCHOR_HORIZONTAL_RIGHT;

		mTextBox = dynamic_cast<TextBox*>(_createWidget(tbd));
		addComponent("TextBoxField",mTextBox);

		mFieldWidget = mTextBox;
	}

	Ogre::String PropertyGridTextProperty::getClass()
	{
		return "PropertyGridTextProperty";
	}

	void PropertyGridTextProperty::updateFieldSkin()
	{
		if((mDesc->propertyGrid != NULL) && (mTextBox != NULL))
			mTextBox->setSkinType(dynamic_cast<PropertyGrid*>(mDesc->propertyGrid)->getTextFieldSkin());
	}
}
